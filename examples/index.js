const { chat } = require('../index');
const { resolve } = require('path');

chat({
  modelPath: resolve(__dirname, '../llms/chatglm3-6b.bin'),
  prompt: '你好',
  callback(type, msg) {
    console.log(type, msg);
  },
});
