import { useState } from 'react';
// import { SidebarNew as Sidebar } from './components/SidebarNew';
// import { Sidebar } from './components/Sidebar';
// import { MarkdownViewer } from './components/MarkdownViewer';
import { ThemeToggle } from './components/ThemeToggle';
// import { useTheme } from './hooks/useTheme';
// import { useMarkdown } from './hooks/useMarkdown';
import { Week } from './types';

// Inline SidebarNew for testing
function Sidebar({ weeks, selectedWeek, onSelectWeek }: { weeks: Week[], selectedWeek: number | null, onSelectWeek: (n: number) => void }) {
  return (
    <div style={{ border: '1px solid red', padding: '20px' }}>
      <h3>Sidebar (No CSS)</h3>
      <ul className="space-y-2">
        {weeks.map(week => (
          <li
            key={week.number}
            onClick={() => onSelectWeek(week.number)}
            className={`p-2 cursor-pointer ${selectedWeek === week.number ? 'bg-blue-500 text-white' : ''}`}
          >
            {week.title}
          </li>
        ))}
      </ul>
    </div>
  );
}

// Generate weeks 1-32
const weeks: Week[] = Array.from({ length: 32 }, (_, i) => ({
  number: i + 1,
  title: `Week ${i + 1}`,
}));

function App() {
  // const [selectedWeek, setSelectedWeek] = useState<number | null>(null);
  const selectedWeek = null;
  const setSelectedWeek = (n: number) => console.log(n);

  return (
    <div className="flex h-screen overflow-hidden">
      <Sidebar
        weeks={weeks}
        selectedWeek={selectedWeek}
        onSelectWeek={setSelectedWeek}
      />
      <div className="flex-1 flex flex-col">
        <div className="p-10">
          <div className="flex justify-between items-center mb-4">
            <h1 className="text-2xl font-bold">Debug Mode</h1>
            <ThemeToggle theme="light" toggleTheme={() => console.log('toggle')} />
          </div>
          <ul className="list-disc pl-5">
            {weeks.map(week => (
              <li
                key={week.number}
                onClick={() => setSelectedWeek(week.number)}
                className="cursor-pointer hover:text-blue-500"
              >
                {week.title}
              </li>
            ))}
          </ul>
        </div>
      </div>
    </div>
  );
}

export default App;
