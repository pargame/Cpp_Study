import { Loader2, AlertCircle } from 'lucide-react';

interface MarkdownViewerProps {
    content: string;
    loading: boolean;
    error: string | null;
    theme: 'light' | 'dark';
}

export function MarkdownViewer({ content, loading, error, theme }: MarkdownViewerProps) {
    if (loading) {
        return (
            <div className="flex items-center justify-center h-full">
                <Loader2 className="w-8 h-8 animate-spin text-primary-500" />
            </div>
        );
    }

    if (error) {
        return (
            <div className="flex items-center justify-center h-full">
                <div className="flex items-center gap-2 text-red-500">
                    <AlertCircle className="w-6 h-6" />
                    <span>{error}</span>
                </div>
            </div>
        );
    }

    if (!content) {
        return (
            <div className="flex items-center justify-center h-full text-gray-500">
                왼쪽에서 주차를 선택해주세요
            </div>
        );
    }

    return (
        <div className="prose dark:prose-invert max-w-none p-8">
            <pre className="whitespace-pre-wrap font-mono text-sm">
                {content}
            </pre>
        </div>
    );
}
