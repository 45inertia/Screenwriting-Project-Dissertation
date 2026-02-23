# Technical Research

Use of framework has been slimmed down to between the Qt framework and Electron.

## Electron
- Chromium and Node.js
- Supports Windows, Mac, Linux
- Customize through html, css, javascript.
- Can use React or Angular

## Qt Framework
- C++ framework
- Supports Windows, Mac, Linux, Android, iOS
- Customize though widgets and Creator Studio.
- Qt ide and tools (requires c++ knowledge)

## Comparison

### Qt Framework (Selected)
**Advantages**
- Compiles into machine code allowing for **low input latency**.
- Qt does not require a browser overhead allowing the application to handle large documents with **memory efficiency**.
- **Interoperability** as there are robust C++ libraries for parsing XML data and handling low-level file system operations.
- **Qt academy** for all Qt learning resources.

**Drawbacks**
- **Manual Memory Management** that requires use of pointers to prevent memory leaks.
- Styling native widgets is more time consuming than CSS based electron styling.

### Electron Framework (Rejected)
**Advantages**
- Lower entry barrier using HTML, CSS and Javascript
  
**Drawbacks**
- Potential high idle memory consumption leading to **resource bloat**.
- The bridge between the renderer and main process invloves more **security risks**.


## External References

[SoftwareLogic](https://softwarelogic.co/en/blog/is-electronjs-better-than-qt-for-modern-ui-development)
states that "The choice between Electron.js and Qt often hinges on the developer's preferred stack 
and project goals."

Software logic also points out the learning curve of Qt: "Qt, on the other hand, requires knowledge
of C++ or QML, which might be less accessible to beginners but offers greater power and 
flexibility."

"but Qt has built-in support for multiple languages and locales, making it a strong choice for 
global applications."

[Winbuzzer](https://winbuzzer.com/2024/05/08/why-choose-qt-the-qt-advantage-in-software-development-xcxwgp/)

[StackShare](https://stackshare.io/stackups/electron-vs-qt) on performance: "Electron applications 
are known to consume more system resources compared to Qt applications. This is because Electron 
applications run a full Chromium browser instance in the background. Qt applications, being written
in C++, have a smaller memory footprint and are generally more efficient in terms of performance."