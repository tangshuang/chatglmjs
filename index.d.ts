export type ChatParams = {
    model_path: string;
    prompt: string;
    temperature?: number;
    top_p?: number;
    top_k?: number;
    onmessage?: (msg: string) => void;
    onend?: () => void;
    onerror?: (e: Error) => void;
};

export declare function chat(params: ChatParams): void;

export declare function chatSync(params: ChatParams): string;