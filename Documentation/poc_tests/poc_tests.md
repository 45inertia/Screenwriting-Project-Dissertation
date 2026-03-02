# Proof of Concept Tests

- [Proof of Concept Tests](#proof-of-concept-tests)
  - [To Do List Application (Console)](#to-do-list-application-console)
      - [Choice of Parser](#choice-of-parser)
      - [Other Conclusions](#other-conclusions)
  - [To Do List Application (Qt)](#to-do-list-application-qt)
    - [XML serialize and deserialize workflow](#xml-serialize-and-deserialize-workflow)


## To Do List Application (Console)
This application was done in the hopes that as it uses simpler technology than a qt application that
a grasp of the overall technologies and workflow could be experimented with.

#### Choice of Parser

For this project a **DOM** (Document Object Model) parser was used as opposed to a **SAX** (Simple 
API for XML) parser. This was done as it was deemed the easiest method for integration as the main
objective for this project was to understand how the XML data would be leveraged in the application
with classes and objects. 

Understanding the difference required research that follows:

[Source: Geeks for Geeks](https://www.geeksforgeeks.org/java/difference-between-sax-parser-and-dom-parser-in-java/)

SAX "uses an event-driven serial-access mechanism for accessing XML documents and is frequently used 
by applets that need to access XML documents because it is the fastest and least memory-consuming 
API available for parsing XML documents."

Whereas DOM "reads an entire document". "It is useful when reading small to medium XML files" and
"occupies more space when loaded into memorr. We can insert and delete nodes using the DOM API."

This source also provides the Features, Advantages and Disadvantages of using DOM vs SAX. These
insights will inform the way XML data is handeled in the final project.

**SAX Parser**
Features:
- The internal structure can not be created by a SAX Parser.
- These event-based SAX parsers work the same as the event handler in java.

Advantages of SAX Parser:
- Simple to use and has good efficiency of memory.
- Its runtime is fast and it can work for bigger documents or file systems.

Disadvantages of SAX Parser:
- Its ability to understand API's is less than an event based API.
- We can't know the full information as there are lots of pieces of data.

**DOM Parser**
Features:
- The internal structure can be created by a DOM parser.
- Due to the internal structure the client can get information about the original XML docs.

Advantages:
- DOM API is easy to use for write and read operations.

Disadvantages:
- It is slow and not memory efficient in comparison to a SAX Parse.

[Source: Geeks for Geeks](https://www.geeksforgeeks.org/java/what-is-sax-in-xml/)

Another source was deemed necessary to understand how the underlying data flows when using a SAX
parser. "SAX processes documents state independently". This means that it does not remember what it
just saw and treats all data independently. This means the burden of building state relies on the
programmer. This benefits the memory usage of the application.

**Conclusion**
From this information a SAX style parser will be used in the final project as it **requires less
memory** and the fact that large projects will struggle when using a DOM parser.

This is when `QXMLStreamReader` was discovered as a library that is native in Qt. 

[Source: Qt](https://doc.qt.io/qt-6/qxmlstreamreader.html)

"The basic concept of a stream reader is to report an XML document as a stream of tokens, similar 
to SAX."

Another feature the `QXMLStreamReader` provides: "The big advantage of this pulling approach is the 
possibility to build recursive descent parsers with it, meaning you can split your XML parsing code
easily into different methods or classes. This makes it easy to keep track of the application's own 
state when parsing XML."

**Conclusion**
This Qt feature will be used for the data handling as it is good for memory management and allows 
control over building the state.

#### Other Conclusions

This first test application was also extremely useful for practicing C++ syntax and development of
an Object Oriented Program.

## To Do List Application (Qt)

### XML serialize and deserialize workflow

**Serialization Worflow**
- Trigger: App calls `XmlSerializer::serialize(file, vector)`.
- Open Pipe: `QFile` opens the hardware level connection to the disk.
- Handshake: `QXmlStreamWriter` attatches to the file.
- Loop: For every `Task` object in vector, writer generates a block of text.
- Cleanup: The file is closed and Writer Destroyed.

**Deserialization Workflow**
- Trigger: App calls `XmlSerializer::deserialize(file, vector)`.
- Scan: `QXmlStreamReader` starts at the first byte of the file.
- Token Check: Reader sees tag and asks if its `Task`.
- Extract: If yes, it pulls the attributes and creates a Task object in the vector.
- Finish: Repeats until `atEnd()`



