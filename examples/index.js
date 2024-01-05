const { chat } = require('../index');
const { resolve } = require('path');

const on = chat({
  modelPath: resolve(__dirname, '../../chatglm2-ggml.bin'),
  prompt: '你好',
});

on('data', (msg) => {
  console.log(msg);
});

on('end', () => {
  console.log('(END)');
});
