const { chat } = require('../index');
const { resolve } = require('path');

chat({
  modelpath: resolve(__dirname, '../llms/chatglm3-6b.bin'),
  prompt: '你好',
  onmessage(msg) {
    process.stdout.write(msg);
  },
});
