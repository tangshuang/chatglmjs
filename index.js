const chatglm = require('bindings')('chatglmjs');
const fs = require('fs');

function chat({
  model_path,
  prompt,
  onmessage,
  onend,
  temperature = 0.95,
  top_p = 0.7,
  top_k = 0,
} = {}) {
  if (!fs.existsSync(model_path)) {
    throw new Error(`model_path: ${model_path} is not existing.`);
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

  chatglm.chat(callback, model_path, prompt, temperature, top_p, top_k);
}

module.exports = { chat };
