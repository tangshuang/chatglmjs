const chatglm = require('bindings')('chatglmjs');
const fs = require('fs');

const createChatFn = (method) => function chatSync({
  model_bin_path,
  prompt,
  temperature = 0.95,
  top_p = 0.7,
  top_k = 0,
  onmessage,
  onend,
  onerror,
} = {}) {
  const warn = (err) => onerror ? onerror(err) : console.error(err);
  if (!fs.existsSync(model_bin_path)) {
    warn(`model_bin_path: ${model_bin_path} is not existing.`);
    return;
  }
  if (!prompt?.trim()) {
    warn('prompt should not be empty.');
    return;
  }

  const callback = (type, msg) => {
    if (type === 'data') {
      onmessage?.(msg);
    }
    else if (type === 'end') {
      onend?.();
    }
  };

  const output = chatglm[method](callback, model_bin_path, prompt, temperature, top_p, top_k);
  return output;
};

const chat = createChatFn('chat');
const chatSync = createChatFn('chatSync');

module.exports = { chat, chatSync };
