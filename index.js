const chatglm = require('bindings')('chatglmjs');
const fs = require('fs');
const { Worker, isMainThread, parentPort, workerData } = require('worker_threads');

function chatSync({
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

  const output = chatglm.chatSync(callback, model_bin_path, prompt, temperature, top_p, top_k);
  return output;
}

if (isMainThread) {
  function chat({
    model_bin_path,
    prompt,
    temperature = 0.95,
    top_p = 0.7,
    top_k = 0,
    onmessage,
    onend,
    onerror,
  } = {}) {
    const workerData = JSON.stringify({ model_bin_path, prompt, temperature, top_p, top_k });
    const worker = new Worker(__filename, { workerData, synchronizedStdio: true });

    worker.on('message', (message) => {
      const data = JSON.parse(message);
      const { type, msg } = data;
      if (type === 'data') {
        onmessage?.(msg);
      }
      else if (type === 'end') {
        onend?.();
      }
    });

    worker.on('error', (e) => {
      onerror?.(e);
    });
  }

  module.exports = { chat, chatSync };
} else {
  const data = JSON.parse(workerData);
  const params = {
    ...data,
    onmessage(msg) {
      parentPort.postMessage(JSON.stringify({ type: 'data', msg }));
    },
    onend() {
      parentPort.postMessage(JSON.stringify({ type: 'end' }));
    },
    onerror(err) {
      throw new Error(err);
    }
  };
  chatSync(params);
  process.exit(0);
}
