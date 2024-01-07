const chatglm = require('bindings')('chatglmjs');
const fs = require('fs');

function chat({ modelpath, prompt, onmessage, onend }) {
  if (!fs.existsSync(modelpath)) {
    throw new Error(`modelpath: ${modelpath} is not existing.`);
  }
  if (!prompt.trim()) {
    throw new Error('prompt should not be empty.');
  }

  const callback = (type, msg) => {
    if (type === 'data') {
      onmessage?.(msg);
    }
    else if (type === 'end') {
      onend?.();
    }
  };

  chatglm.chat(callback, modelpath, prompt);
}

module.exports = { chat };
