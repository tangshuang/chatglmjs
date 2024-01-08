export type ChatParams = {
    /** the converted LLM model file path */
    model_path: string;
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

export declare function chat(params: ChatParams): void;

export declare function chatSync(params: ChatParams): string;