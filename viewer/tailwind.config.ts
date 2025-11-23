import type { Config } from 'tailwindcss';

const config: Config = {
  darkMode: 'class',
  content: ['./index.html', './src/**/*.{ts,tsx}'],
  theme: {
    extend: {
      fontFamily: {
        sans: ['"Inter"', 'system-ui', 'sans-serif']
      },
      colors: {
        canvas: {
          light: '#f8fafc',
          dark: '#0f172a'
        },
        surface: {
          light: '#ffffff',
          dark: '#1e293b'
        },
        accent: '#4f46e5'
      }
    }
  }
};

export default config;
