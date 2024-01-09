const chatglm = require('bindings')('chatglmjs');
const fs = require('fs');

const createChatFn = (method, defered) => function chatSync({
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

  let resolve, reject, message = '';
  const promise = new Promise((r, j) => {
    resolve = r;
    reject = j;
  });

  const callback = (type, msg) => {
    if (type === 'data') {
      onmessage?.(msg);
      message += msg;
    }
    else if (type === 'end') {
      onend?.();
      resolve(message);
    }
    else if (type === 'error') {
      onerror?.(msg);
      reject(new Error(msg));
    }
  };

  const output = chatglm[method](callback, model_bin_path, prompt, temperature, top_p, top_k);
  return defered ? promise : output;
};

const chat = createChatFn('chat', true);
const chatSync = createChatFn('chatSync');

module.exports = { chat, chatSync };
