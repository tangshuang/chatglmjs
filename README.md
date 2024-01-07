# ChatGLM.js

ChatGLM NodeJS addon.
You can use a ChatGLM LLM easily in nodejs or electron application.

## Usage

```
npm i chatglmjs
```

```js
const { chat } = require('../index');
const { resolve } = require('path');

chat({
  modelpath: resolve(__dirname, '../llms/chatglm3-6b.q4_0.apple-macbook-m2pro.bin'),
  prompt: '你好',
  onmessage(msg) {
    process.stdout.write(msg);
  },
});
```

- modelpath: the converted LLM model file path (learn more latter)
- prompt
- onmessage: callback function with the answered text by LLM
- onend: callback function when LLM answer end

## LLM

The addon is created based on [chatglm.cpp](https://github.com/li-plus/chatglm.cpp) project. I used the c++ source code to build a nodejs extension. So you should learn about chatglm.cpp to create converted LLM model files.

```
git clone --recursive https://github.com/li-plus/chatglm.cpp.git && cd chatglm.cpp
python3 -m pip install -U pip
python3 -m pip install torch tabulate tqdm transformers accelerate sentencepiece
python3 chatglm_cpp/convert.py -i THUDM/chatglm-6b -t q4_0 -o chatglm3-6b.q4_0.apple-macbook-m2pro.bin
```

Then you will get a `chatglm3-6b.q4_0.apple-macbook-m2pro.bin` file, use this file as converted LLM model file and pass its file path to `modelpath` option.

chatglm.cpp supports the models `ChatGLM2-6B` `ChatGLM3-6B` `Baichuan-13B-Chat` `Baichuan2-7B-Chat` `Baichuan2-13B-Chat` `InternLM-Chat-7B` `InternaL-Chat-20B` and so on models. You can read its document to find out which models supported.

I suggeust you keep converted model file name with `[model-name].[quantization].[device].bin` formatter so that you can share it to other developers. *If you have converted some files, maybe you will share it in issues, so that other developers can use it directly on certain device.*

ChatGLM.js only run LLM on CPU, so you should make sure your computer device is enough for the target model. `THUDM/chatglm-6b-int4` is recommanded, it can even run on android mobile phone with termux.

# MIT License

Copyright (c) 2024 Shuang Tang

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
