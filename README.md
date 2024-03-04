# ChatGLM.js

ChatGLM NodeJS addon.
You can run a local ChatGLM LLM easily in nodejs or electron application.

## Usage

Before installation, make sure your device environment is able to compile c++ source code. We will use cmake-js to build a .node file.

```
npm i chatglmjs
```

Requirements:

- Windows:
  - [CMake](https://cmake.org/download/)
  - Visutal Studio with C/C++ compiler or [more for cmake-js](https://github.com/cmake-js/cmake-js?tab=readme-ov-file#installation), *Notice: Visutal Studio should be open when installing*
  - [Visual C++ Build Tools](https://visualstudio.microsoft.com/zh-hans/visual-cpp-build-tools/)
- Unix/Posix:
  - gcc/g++
  - cmake

```js
const { chat, chatSync } = require('chatglmjs');
const { resolve } = require('path');

chat({
  model_bin_path: resolve(__dirname, '../llms/chatglm3-6b.q4_0.bin'),
  prompt: '你好',
  onmessage(msg) {
    process.stdout.write(msg);
  },
});
```

```ts
type ChatParams = {
    /** the converted LLM model bin file path */
    model_bin_path: string;
    prompt: string;
    /** @default 0.95 */
    temperature?: number;
    /** @default 0.7 */
    top_p?: number;
    /** @default 0 */
    top_k?: number;
    /** callback function with the answered text by LLM */
    onmessage?: (msg: string) => void;
    /** callback function when LLM answer end */
    onend?: () => void;
    /** callback function when error ocurs */
    onerror?: (e: Error) => void;
};
```

## LLM

The addon is created based on [chatglm.cpp](https://github.com/li-plus/chatglm.cpp) project. I used the c++ source code to build a nodejs extension. So you should learn about chatglm.cpp to create converted LLM model files.

```
git clone --recursive https://github.com/li-plus/chatglm.cpp.git && cd chatglm.cpp
python3 -m pip install -U pip
python3 -m pip install torch tabulate tqdm transformers accelerate sentencepiece
python3 chatglm_cpp/convert.py -i THUDM/chatglm3-6b -t q4_0 -o chatglm3-6b.q4_0.bin
```

[Download a biult of chatglm3-6b.q4_0.bin from network disk.](https://pan.baidu.com/s/14Pi-egjtnuqBGl8NAanwpg?pwd=s2sv)

Then you will get a `chatglm3-6b.q4_0.bin` file, use this file as converted LLM model file and pass its file path to `model_bin_path` option.

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
