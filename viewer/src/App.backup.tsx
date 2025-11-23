import { useState, useEffect } from 'react';
import { Sidebar } from './components/Sidebar';
import { MarkdownViewer } from './components/MarkdownViewer';
import { ThemeToggle } from './components/ThemeToggle';
import { useTheme } from './hooks/useTheme';
import { useMarkdown } from './hooks/useMarkdown';
import { Week } from './types';

// Generate weeks 1-32
const weeks: Week[] = Array.from({ length: 32 }, (_, i) => ({
  number: i + 1,
  title: `Week ${i + 1}`, // Will be updated when README is loaded
}));

function App() {
  const [selectedWeek, setSelectedWeek] = useState<number | null>(null);
  const { theme, toggleTheme } = useTheme();
  const { content, loading, error } = useMarkdown(selectedWeek);

  // Handle URL hash routing
  useEffect(() => {
    const handleHashChange = () => {
      const hash = window.location.hash;
      const match = hash.match(/#\/week\/(\d+)/);
      if (match) {
        const weekNum = parseInt(match[1]);
        if (weekNum >= 1 && weekNum <= 32) {
          setSelectedWeek(weekNum);
        }
      }
    };

    handleHashChange();
    window.addEventListener('hashchange', handleHashChange);
    return () => window.removeEventListener('hashchange', handleHashChange);
  }, []);

  // Update URL when week is selected
  const handleSelectWeek = (weekNumber: number) => {
    setSelectedWeek(weekNumber);
    window.location.hash = `#/week/${weekNumber}`;
  };

  // Extract title from README content
  useEffect(() => {
    if (content && selectedWeek) {
      const firstLine = content.split('\n')[0];
      const titleMatch = firstLine.match(/^#\s+(.+)/);
      if (titleMatch) {
        const weekIndex = weeks.findIndex(w => w.number === selectedWeek);
        if (weekIndex !== -1) {
          weeks[weekIndex].title = titleMatch[1].replace(/^\d+주차:\s*/, '');
        }
      }
    }
  }, [content, selectedWeek]);

  return (
    <div className="flex h-screen overflow-hidden">
      <Sidebar
        weeks={weeks}
        selectedWeek={selectedWeek}
        onSelectWeek={handleSelectWeek}
      />

      <div className="flex-1 flex flex-col">
        <header className="h-16 border-b border-gray-300 dark:border-gray-700 flex items-center justify-between px-6 bg-white dark:bg-gray-800">
          <h2 className="text-lg font-semibold">
            {selectedWeek ? `Week ${selectedWeek}` : 'C++ 멀티쓰레딩 & 네트워크 프로그래밍'}
          </h2>
          <ThemeToggle theme={theme} toggleTheme={toggleTheme} />
        </header>

        <main className="flex-1 overflow-y-auto bg-white dark:bg-gray-900">
          <MarkdownViewer
            content={content}
            loading={loading}
            error={error}
            theme={theme}
          />
        </main>
      </div>
    </div>
  );
}

export default App;
