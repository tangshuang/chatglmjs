const { chatSync } = require('../index');
const { resolve } = require('path');

const output = chatSync({
    model_path: resolve(__dirname, '../llms/chatglm3-6b.bin'),
    prompt: '你好',
});

console.log(output);