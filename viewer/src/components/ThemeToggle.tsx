// import { Moon, Sun } from 'lucide-react';
import { Theme } from '../types';

interface ThemeToggleProps {
    theme: Theme;
    toggleTheme: () => void;
}

export function ThemeToggle({ theme, toggleTheme }: ThemeToggleProps) {
    return (
        <button
            onClick={toggleTheme}
            className="p-2 rounded-lg hover:bg-gray-100 dark:hover:bg-gray-700 transition-colors"
            aria-label="Toggle theme"
        >
            {theme === 'light' ? (
                // <Moon className="w-5 h-5 text-gray-600 dark:text-gray-300" />
                <span>Moon</span>
            ) : (
                // <Sun className="w-5 h-5 text-yellow-500" />
                <span>Sun</span>
            )}
        </button>
    );
}
