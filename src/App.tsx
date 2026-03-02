import React from 'react';
import { Layers, Mic, Search, MessageSquare, Shield, Code, Terminal, Cpu, Zap, FolderTree, CheckCircle2, Server, Globe, Database } from 'lucide-react';
import { projectData } from './data';
import { CodeBlock } from './components/CodeBlock';
import { motion } from 'motion/react';

const iconMap: Record<string, React.ElementType> = {
  Layers, Mic, Search, MessageSquare, Shield, Code
};

export default function App() {
  return (
    <div className="min-h-screen bg-az-dark text-gray-300 selection:bg-az-blue/30">
      {/* Navigation */}
      <nav className="sticky top-0 z-50 border-b border-az-border bg-az-dark/80 backdrop-blur-md">
        <div className="max-w-7xl mx-auto px-6 h-16 flex items-center justify-between">
          <div className="flex items-center gap-2">
            <Terminal className="w-6 h-6 text-az-blue" />
            <span className="font-bold text-lg text-white tracking-tight">{projectData.name}</span>
          </div>
          <div className="flex items-center gap-6 text-sm font-medium">
            <a href="#features" className="hover:text-white transition-colors">Features</a>
            <a href="#architecture" className="hover:text-white transition-colors">Architecture</a>
            <a href="#code" className="hover:text-white transition-colors">Implementation</a>
            <a href="#deployment" className="hover:text-white transition-colors">Deployment</a>
          </div>
        </div>
      </nav>

      <main>
        {/* Hero Section */}
        <section className="relative pt-32 pb-20 overflow-hidden">
          <div className="absolute inset-0 bg-[radial-gradient(ellipse_at_top,_var(--tw-gradient-stops))] from-az-blue/20 via-az-dark to-az-dark -z-10" />
          <div className="max-w-7xl mx-auto px-6">
            <motion.div 
              initial={{ opacity: 0, y: 20 }}
              animate={{ opacity: 1, y: 0 }}
              transition={{ duration: 0.5 }}
              className="max-w-3xl"
            >
              <div className="flex items-center gap-3 mb-6">
                <span className="px-3 py-1 rounded-full border border-az-blue/30 bg-az-blue/10 text-az-blue text-xs font-mono font-medium">
                  {projectData.language}
                </span>
                <span className="px-3 py-1 rounded-full border border-gray-700 bg-gray-800/50 text-gray-300 text-xs font-mono font-medium">
                  {projectData.type}
                </span>
              </div>
              <h1 className="text-5xl md:text-7xl font-bold text-white tracking-tight mb-6 leading-tight">
                Native Azure AI <br />
                <span className="text-transparent bg-clip-text bg-gradient-to-r from-az-blue to-cyan-400">
                  in your terminal.
                </span>
              </h1>
              <p className="text-lg md:text-xl text-gray-400 leading-relaxed mb-10 max-w-2xl">
                {projectData.description}
              </p>
              <div className="flex items-center gap-4">
                <button className="px-6 py-3 rounded-lg bg-az-blue text-white font-medium hover:bg-blue-600 transition-colors flex items-center gap-2">
                  <Terminal className="w-5 h-5" />
                  View Documentation
                </button>
                <button className="px-6 py-3 rounded-lg border border-az-border bg-az-card hover:bg-gray-800 text-white font-medium transition-colors flex items-center gap-2">
                  <Code className="w-5 h-5" />
                  Source Code
                </button>
              </div>
            </motion.div>
          </div>
        </section>

        {/* Features Section */}
        <section id="features" className="py-24 border-t border-az-border bg-az-card/30">
          <div className="max-w-7xl mx-auto px-6">
            <div className="mb-16">
              <h2 className="text-3xl font-bold text-white mb-4">Key Capabilities</h2>
              <p className="text-gray-400 max-w-2xl">Designed for seamless integration into existing shell scripts and automated workflows.</p>
            </div>
            <div className="grid grid-cols-1 md:grid-cols-2 lg:grid-cols-3 gap-6">
              {projectData.keyFeatures.map((feature, idx) => {
                const Icon = iconMap[feature.icon] || CheckCircle2;
                return (
                  <motion.div 
                    key={idx}
                    initial={{ opacity: 0, y: 20 }}
                    whileInView={{ opacity: 1, y: 0 }}
                    viewport={{ once: true }}
                    transition={{ delay: idx * 0.1 }}
                    className="p-6 rounded-2xl border border-az-border bg-az-card hover:border-az-blue/50 transition-colors group"
                  >
                    <div className="w-12 h-12 rounded-xl bg-az-blue/10 flex items-center justify-center mb-6 group-hover:scale-110 transition-transform">
                      <Icon className="w-6 h-6 text-az-blue" />
                    </div>
                    <h3 className="text-lg font-semibold text-white mb-2">{feature.title}</h3>
                    <p className="text-sm text-gray-400 leading-relaxed">{feature.description}</p>
                  </motion.div>
                );
              })}
            </div>
          </div>
        </section>

        {/* Architecture & Performance */}
        <section id="architecture" className="py-24 border-t border-az-border">
          <div className="max-w-7xl mx-auto px-6">
            <div className="grid grid-cols-1 lg:grid-cols-2 gap-16">
              <div>
                <h2 className="text-3xl font-bold text-white mb-6">Architecture Overview</h2>
                <p className="text-gray-400 leading-relaxed mb-8">
                  {projectData.architectureOverview}
                </p>
                
                <h3 className="text-xl font-semibold text-white mb-4 flex items-center gap-2">
                  <Zap className="w-5 h-5 text-yellow-500" />
                  Performance Strategy
                </h3>
                <p className="text-gray-400 leading-relaxed mb-8">
                  {projectData.performanceRequirements}
                </p>

                <h3 className="text-xl font-semibold text-white mb-4 flex items-center gap-2">
                  <Database className="w-5 h-5 text-green-500" />
                  Recommended Stack
                </h3>
                <ul className="grid grid-cols-1 sm:grid-cols-2 gap-3">
                  {projectData.recommendedLibraries.map((lib, idx) => (
                    <li key={idx} className="flex items-center gap-2 text-sm text-gray-300 bg-az-card px-3 py-2 rounded-lg border border-az-border">
                      <CheckCircle2 className="w-4 h-4 text-az-blue flex-shrink-0" />
                      <span>{lib}</span>
                    </li>
                  ))}
                </ul>
              </div>

              {/* Folder Structure */}
              <div className="bg-az-card border border-az-border rounded-2xl p-6 flex flex-col">
                <h3 className="text-lg font-semibold text-white mb-4 flex items-center gap-2">
                  <FolderTree className="w-5 h-5 text-gray-400" />
                  Project Structure
                </h3>
                <div className="flex-1 overflow-auto bg-[#0a0a0a] rounded-xl border border-az-border p-4">
                  <pre className="font-mono text-sm text-gray-400 leading-relaxed">
                    {projectData.folderStructure}
                  </pre>
                </div>
              </div>
            </div>
          </div>
        </section>

        {/* Code Snippets */}
        <section id="code" className="py-24 border-t border-az-border bg-az-card/30">
          <div className="max-w-7xl mx-auto px-6">
            <div className="mb-16">
              <h2 className="text-3xl font-bold text-white mb-4">Core Implementation</h2>
              <p className="text-gray-400 max-w-2xl">A look at the underlying C++ 20 implementation leveraging CLI11, CPR, and nlohmann/json.</p>
            </div>
            
            <div className="space-y-16">
              {projectData.codeSnippets.map((snippet, idx) => (
                <motion.div 
                  key={idx}
                  initial={{ opacity: 0, y: 20 }}
                  whileInView={{ opacity: 1, y: 0 }}
                  viewport={{ once: true }}
                >
                  <CodeBlock 
                    title={snippet.title}
                    description={snippet.description}
                    language={snippet.language}
                    code={snippet.code}
                  />
                </motion.div>
              ))}
            </div>
          </div>
        </section>

        {/* Deployment & Testing */}
        <section id="deployment" className="py-24 border-t border-az-border">
          <div className="max-w-7xl mx-auto px-6">
            <div className="grid grid-cols-1 md:grid-cols-2 gap-12">
              <div className="p-8 rounded-2xl border border-az-border bg-az-card">
                <div className="w-12 h-12 rounded-xl bg-purple-500/10 flex items-center justify-center mb-6">
                  <Shield className="w-6 h-6 text-purple-500" />
                </div>
                <h3 className="text-xl font-bold text-white mb-4">Testing Strategy</h3>
                <p className="text-gray-400 leading-relaxed">
                  {projectData.testingStrategy}
                </p>
              </div>
              <div className="p-8 rounded-2xl border border-az-border bg-az-card">
                <div className="w-12 h-12 rounded-xl bg-green-500/10 flex items-center justify-center mb-6">
                  <Globe className="w-6 h-6 text-green-500" />
                </div>
                <h3 className="text-xl font-bold text-white mb-4">Deployment Plan</h3>
                <p className="text-gray-400 leading-relaxed">
                  {projectData.deploymentPlan}
                </p>
              </div>
            </div>
          </div>
        </section>
      </main>

      <footer className="border-t border-az-border py-12 bg-az-card">
        <div className="max-w-7xl mx-auto px-6 flex flex-col md:flex-row items-center justify-between gap-4">
          <div className="flex items-center gap-2">
            <Terminal className="w-5 h-5 text-gray-500" />
            <span className="font-semibold text-gray-400">{projectData.name}</span>
          </div>
          <p className="text-sm text-gray-500">
            Built with C++ 20 for maximum performance.
          </p>
        </div>
      </footer>
    </div>
  );
}
