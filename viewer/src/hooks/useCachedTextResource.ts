import { MutableRefObject, useCallback, useEffect, useState } from 'react';

const isAbortError = (error: unknown): error is DOMException =>
  error instanceof DOMException && error.name === 'AbortError';

const asErrorMessage = (error: unknown, fallback: string) => (error instanceof Error ? error.message : fallback);

type UseCachedTextResourceOptions = {
  key: string | null;
  cacheRef: MutableRefObject<Map<string, string>>;
  shouldCache: boolean;
  enabled?: boolean;
  errorMessage: string;
};

type UseCachedTextResourceResult = {
  data: string;
  loading: boolean;
  error: string | null;
  retry: () => void;
};

const useCachedTextResource = ({
  key,
  cacheRef,
  shouldCache,
  enabled = true,
  errorMessage
}: UseCachedTextResourceOptions): UseCachedTextResourceResult => {
  const [data, setData] = useState('');
  const [loading, setLoading] = useState(false);
  const [error, setError] = useState<string | null>(null);
  const [reloadToken, setReloadToken] = useState(0);

  const retry = useCallback(() => {
    if (!key) {
      return;
    }
    if (shouldCache) {
      cacheRef.current.delete(key);
    }
    setError(null);
    setReloadToken((token) => token + 1);
  }, [cacheRef, key, shouldCache]);

  useEffect(() => {
    if (!enabled || !key) {
      setLoading(false);
      setError(null);
      setData('');
      return;
    }

    const cached = shouldCache ? cacheRef.current.get(key) : undefined;
    if (cached !== undefined) {
      setData(cached);
      setLoading(false);
      return;
    }

    const controller = new AbortController();
    let canceled = false;
    setLoading(true);

    const fetchResource = async () => {
      try {
        const response = await fetch(key, { signal: controller.signal });
        if (!response.ok) {
          throw new Error(errorMessage);
        }
        const text = await response.text();
        if (canceled) {
          return;
        }
        if (shouldCache) {
          cacheRef.current.set(key, text);
        }
        setData(text);
        setError(null);
      } catch (fetchError) {
        if (canceled || isAbortError(fetchError)) {
          return;
        }
        setError(asErrorMessage(fetchError, errorMessage));
      } finally {
        if (!canceled) {
          setLoading(false);
        }
      }
    };

    fetchResource();

    return () => {
      canceled = true;
      controller.abort();
    };
  }, [cacheRef, enabled, errorMessage, key, reloadToken, shouldCache]);

  return {
    data,
    loading,
    error,
    retry
  };
};

export default useCachedTextResource;
