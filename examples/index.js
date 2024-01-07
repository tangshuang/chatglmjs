const { chat } = require('../index');
const { resolve } = require('path');

chat({
  model_path: resolve(__dirname, '../llms/chatglm3-6b.bin'),
  prompt: '你好',
  onmessage(msg) {
    process.stdout.write(msg);
  },
});
