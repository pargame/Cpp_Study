// import { Moon, Sun } from 'lucide-react';
import { Theme } from '../types';

interface ThemeToggleProps {
    theme: Theme;
    toggleTheme: () => void;
}

export function ThemeToggle2({ theme, toggleTheme }: ThemeToggleProps) {
    return (
        <button
            onClick={toggleTheme}
            className="p-2 rounded-lg bg-gray-200 dark:bg-gray-700 hover:bg-gray-300 dark:hover:bg-gray-600 transition-colors"
            aria-label="Toggle theme"
        >
            {theme === 'light' ? (
                <span className="text-yellow-500 font-bold">SUN</span>
            ) : (
                <span className="text-blue-400 font-bold">MOON</span>
            )}
        </button>
    );
}
