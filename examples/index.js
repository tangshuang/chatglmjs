const { chat } = require('../index');
const { resolve } = require('path');
const dotenv = require('dotenv');

dotenv.config();

const model_bin_path = process.env.MODEL_BIN_PATH || resolve(__dirname, '../llms/chatglm3-6b.bin');

async function init() {
  const message = await chat({
    model_bin_path,
    prompt: '你好',
    onmessage(msg) {
      process.stdout.write(msg);
    },
    onend() {
      console.log('==End==');
    },
    onerror(e) {
      console.error(e);
    },
  });

  console.log('Promise Output::', message);
}

init();