import React from 'react';
import ReactDOM from 'react-dom/client';
import App from './App';
import './index.css';

const enableDarkMode = () => {
  const root = document.documentElement;
  root.classList.add('dark');
  root.style.colorScheme = 'dark';
  document.body.classList.add('bg-slate-950');
  document.body.classList.add('text-slate-100');
};

enableDarkMode();

ReactDOM.createRoot(document.getElementById('root') as HTMLElement).render(
  <React.StrictMode>
    <App />
  </React.StrictMode>
);
