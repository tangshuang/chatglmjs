const chatglm = require('bindings')('chatglmjs');
const fs = require('fs');

function chat({ modelPath, prompt, callback }) {
  if (!fs.existsSync(modelPath)) {
    throw new Error(`modelPath: ${modelPath} is not existing.`);
  }
  if (!prompt.trim()) {
    throw new Error('prompt should not be empty.');
  }
  if (typeof callback !== 'function') {
    throw new Error('callback should be Function');
  }
  chatglm.chat(callback, modelPath, prompt);
}

module.exports = { chat };
