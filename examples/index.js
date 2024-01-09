const { chat } = require('../index');
const { resolve } = require('path');
const dotenv = require('dotenv');

dotenv.config();

const model_bin_path = process.env.MODEL_BIN_PATH || resolve(__dirname, '../llms/chatglm3-6b.bin');

chat({
  model_bin_path,
  prompt: '你好',
  onmessage(msg) {
    process.stdout.write(msg);
  },
});
