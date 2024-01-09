const { chatSync } = require('../index');
const { resolve } = require('path');
const dotenv = require('dotenv');

dotenv.config();

const model_bin_path = process.env.MODEL_BIN_PATH || resolve(__dirname, '../llms/chatglm3-6b.bin');

const output = chatSync({
    model_bin_path,
    prompt: '你好',
    onerror(e) {
        console.error(e);
    },
});

console.log(output);
