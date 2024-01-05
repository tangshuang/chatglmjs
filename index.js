const { EventEmitter } = require('events');
const chatglm = require('bindings')('chatglm');
const fs = require('fs');

function chat({ modelPath, prompt }) {
  if (!fs.existsSync(modelPath)) {
    throw new Error(`modelPath: ${modelPath} is not existing.`);
  }
  if (!prompt.trim()) {
    throw new Error('prompt should not be empty.');
  }
  const emitter = new EventEmitter();
  chatglm.chat(emitter.emit.bind(emitter), modelPath, prompt);
  return emitter.on.bind(emitter);
}

module.exports = { chat };
