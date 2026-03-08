# AzAI-CLI Agent Documentation

## Overview

AzAI-CLI is an AI Studio application powered by Google Gemini API. It provides a web-based interface for interacting with AI models, allowing users to send queries and receive intelligent responses with code highlighting and formatted output.

## Project Structure

```
AzAI-CLI/
├── src/
│   ├── App.tsx              # Main application component
│   ├── main.tsx             # React entry point
│   ├── data.ts              # Data management utilities
│   ├── index.css            # Global styles
│   ├── components/
│   │   └── CodeBlock.tsx    # Code block rendering component
├── build/                   # Build output directory
├── package.json             # Node.js dependencies and scripts
├── tsconfig.json            # TypeScript configuration
├── vite.config.ts           # Vite bundler configuration
└── index.html               # HTML entry point
```

## Technology Stack

- **Frontend**: React 19, TypeScript, Tailwind CSS
- **Build Tool**: Vite 6.2
- **AI Integration**: Google Gemini API (@google/genai)
- **UI Components**: Lucide React (icons), Motion (animations)
- **Code Display**: React Syntax Highlighter
- **Development**: Node.js, Express (optional backend)

## Setup Instructions

### Prerequisites

- Node.js (18.0 or higher recommended)
- npm or yarn package manager
- Google Gemini API key

### Installation

1. **Clone and navigate to the project**:
   ```bash
   cd AzAI-CLI
   ```

2. **Install dependencies**:
   ```bash
   npm install
   ```

3. **Configure API Key**:
   - Create a `.env.local` file in the project root
   - Add your Gemini API key:
   ```
   VITE_GEMINI_API_KEY=your_api_key_here
   ```

### Running the Application

**Development Mode**:
```bash
npm run dev
```
The app will be available at `http://localhost:3000`

**Production Build**:
```bash
npm run build
```

**Preview Build**:
```bash
npm run preview
```

**Type Checking**:
```bash
npm run lint
```

## Agent Capabilities

### Core Features

1. **Query Processing**: Send natural language queries to the Gemini AI model
2. **Response Formatting**: Receive structured responses with support for:
   - Plain text
   - Code blocks with syntax highlighting
   - Formatted lists and paragraphs
3. **Code Highlighting**: Automatic syntax highlighting for multiple programming languages
4. **Interactive UI**: Real-time interaction with smooth animations

### Component Architecture

#### App.tsx
- Main application container
- Manages conversation state
- Handles API communication with Gemini
- Renders UI components

#### CodeBlock.tsx
- Renders code blocks with syntax highlighting
- Supports multiple programming languages
- Provides copy functionality

#### data.ts
- Utility functions for data manipulation
- API response parsing
- Message formatting

## API Integration

The application uses the Google Generative AI API to power its intelligence.

### Configuration

The API is configured via environment variables:
```javascript
const apiKey = process.env.VITE_GEMINI_API_KEY;
```

### Usage Example

```typescript
import { GoogleGenerativeAI } from "@google/genai";

const genai = new GoogleGenerativeAI(apiKey);
const model = genai.getGenerativeModel({ model: "gemini-pro" });

const response = await model.generateContent(userQuery);
```

## Development Guidelines

### Code Quality
- Use TypeScript for type safety
- Run `npm run lint` to check for type errors
- Follow React best practices and hooks patterns

### Adding New Features
1. Create new components in `src/components/`
2. Update imports in `App.tsx` or parent components
3. Ensure proper TypeScript typing
4. Test functionality before committing

### Styling
- Project uses Tailwind CSS for styling
- Global styles are in `src/index.css`
- Component-specific styles can be added inline or in separate CSS modules

## Build Configuration

### Vite Configuration
The project uses Vite for fast development and optimized production builds:
- Hot Module Replacement (HMR) enabled for development
- React plugin for JSX support
- Tailwind CSS Vite plugin for optimization

### TypeScript Configuration
- Target: ES2020
- Module: ESNext
- Strict mode enabled
- Support for JSX

## Deployment

The application can be deployed to any static hosting service:

1. **Build the project**:
   ```bash
   npm run build
   ```

2. **Deploy the `dist` folder** to your hosting service (Vercel, Netlify, Azure, etc.)

3. **Set environment variables** on your hosting platform:
   - `VITE_GEMINI_API_KEY`: Your Gemini API key

## Troubleshooting

### API Key Issues
- Ensure `.env.local` is created in the project root
- Verify the API key is valid and has Gemini API access enabled
- Check that the key is not exposed in version control

### Build Errors
- Clear node_modules and reinstall: `rm -rf node_modules && npm install`
- Clear Vite cache: `npm run clean`
- Check Node.js version compatibility

### Development Server Issues
- Ensure port 3000 is available
- Try clearing browser cache
- Restart the development server

## Security Considerations

- **API Key Management**: Never commit `.env.local` to version control
- **Environment Variables**: Use `.env.local` for local development
- **Production**: Use secure environment variable management in your hosting platform
- **Dependencies**: Regularly update dependencies for security patches

## Contributing

When contributing to this project:
1. Follow TypeScript and React best practices
2. Maintain existing code style
3. Test changes thoroughly
4. Update documentation as needed

## References

- [Google Generative AI API Documentation](https://ai.google.dev/)
- [React Documentation](https://react.dev/)
- [Vite Documentation](https://vitejs.dev/)
- [Tailwind CSS Documentation](https://tailwindcss.com/)
- [TypeScript Documentation](https://www.typescriptlang.org/)

## AI Studio Integration

This application is integrated with AI Studio at:
https://ai.studio/apps/559ad9b6-ed88-4441-807a-e2d1002026a3

View and manage your app settings through the AI Studio dashboard.

## License

Please refer to the project's license file for licensing information.
