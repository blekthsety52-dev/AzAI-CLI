import React from 'react';
import { Prism as SyntaxHighlighter } from 'react-syntax-highlighter';
import { vscDarkPlus } from 'react-syntax-highlighter/dist/esm/styles/prism';

interface CodeBlockProps {
  code: string;
  language: string;
  title: string;
  description?: string;
}

export function CodeBlock({ code, language, title, description }: CodeBlockProps) {
  return (
    <div className="flex flex-col gap-3">
      <div className="flex flex-col gap-1">
        <h3 className="text-lg font-semibold text-white">{title}</h3>
        {description && <p className="text-sm text-gray-400">{description}</p>}
      </div>
      <div className="rounded-xl overflow-hidden border border-az-border bg-[#1E1E1E] shadow-2xl">
        <div className="flex items-center justify-between px-4 py-2 border-b border-az-border bg-[#252526]">
          <div className="flex items-center gap-2">
            <div className="flex gap-1.5">
              <div className="w-3 h-3 rounded-full bg-red-500/20 border border-red-500/50" />
              <div className="w-3 h-3 rounded-full bg-yellow-500/20 border border-yellow-500/50" />
              <div className="w-3 h-3 rounded-full bg-green-500/20 border border-green-500/50" />
            </div>
            <span className="ml-2 font-mono text-xs text-gray-400">{title.toLowerCase().replace(/\s+/g, '_')}.cpp</span>
          </div>
          <span className="font-mono text-xs text-gray-500 uppercase">{language}</span>
        </div>
        <div className="text-sm">
          <SyntaxHighlighter
            language={language}
            style={vscDarkPlus}
            customStyle={{
              margin: 0,
              padding: '1.5rem',
              background: 'transparent',
              fontSize: '0.875rem',
              lineHeight: '1.5',
            }}
            showLineNumbers={true}
            wrapLines={true}
          >
            {code}
          </SyntaxHighlighter>
        </div>
      </div>
    </div>
  );
}
