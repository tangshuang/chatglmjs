const { EventEmitter } = require('events');
const chatglm = require('bindings')('chatglm');

function chat({ modelPath, prompt }) {
  const emitter = new EventEmitter();
  chatglm.chat(emitter.emit.bind(emitter), modelPath, prompt);
  return emitter.on.bind(emitter);
}

module.exports = { chat };
