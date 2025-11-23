import { useState, useEffect } from 'react';

export function useMarkdown(weekNumber: number | null) {
    const [content, setContent] = useState<string>('');
    const [loading, setLoading] = useState<boolean>(false);
    const [error, setError] = useState<string | null>(null);

    useEffect(() => {
        if (weekNumber === null) {
            setContent('');
            return;
        }

        const loadMarkdown = async () => {
            setLoading(true);
            setError(null);

            try {
                // Construct path to README.md
                const path = `../Week${weekNumber}/README.md`;
                const response = await fetch(path);

                if (!response.ok) {
                    throw new Error(`Failed to load Week ${weekNumber} README`);
                }

                const text = await response.text();
                setContent(text);
            } catch (err) {
                setError(err instanceof Error ? err.message : 'Unknown error occurred');
                setContent('');
            } finally {
                setLoading(false);
            }
        };

        loadMarkdown();
    }, [weekNumber]);

    return { content, loading, error };
}
