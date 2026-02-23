# Survey Questions

This document is the list of questions that I intend to use for the research survey.

### User Profile and Habits
This section aims to gather data on the type of user taking the survey, this can then be used to
provide insights when analysed with other answers from the survey.

- **How would you describe your screenwriting status?**
  - *Options: Student, Hobbyist, Aspiring Professional, Working Professional*
- **Which operating system do you primarily use for writing?**
  - *Options: Windows, Mac, Linux, Browser, Other*
- **What is your current primary screenwriting software?**
  - *Options: Fade In, Final Draft, Scrivener, Celtx, Arc Studio, Other*
- **Have you paid for this software? (subscription/one time purchase)**
  - *Options: Yes, No*
- **How long have you been using your current primary screenwriting software?**
  - *Options: Less than 1 year, 1-3 years, 3-5 years, 5+ years*
- **What is the typical length of the projects you work on?**
  - *Options: Short films/sketches (<30 pages), TV Pilots (30-60 pages), Feature Films 
    (90-120 pages), Long-form/Series(120+ pages)*
- **Do you  primarily write along or as part of a writing team?**
  - *Options: Solo, Collaboration/Team*
- **What is your primary method for sharing your scripts with others?**
  - *Options: PDF, Final Draft (.fdx), Fountain (.fountain) Cloud-link access, Printed hard copy, 
    Other*

### Pain Points
The data gathered from this part of the survey will have a direct effect on the finalised
requirements for the project including feature prioritisation.

- **On a scale of 1-5, how much of a financial burden is your current software subscription or upgrade path (1 if using free)?**
  - *Options: 1 to 5*
  - Relevance: Provides evidence on the value of the product and a potential gap for free software.
- **How often do you experience "typing lag" (delay between hitting a key and text appearing)?**
  - *Options: Never, Rarely, Sometimes, Often, Constantly*
  - Relevance: May directly justify the development using the Qt Framework.
- **Have you ever lost work due to a software crash or sync error using your current screenwriting software?**
  - *Options: Yes, No*
  - Relevance: May justify snapshots and auto save features.
- **On a scale of 1-5, how overwhelmed do you feel by the number of menus and buttons you never use?**
  - *Options: 1 to 5*
  - Relevance: May justify focus mode feature and use of UI Principles.
- **Have you ever been unable to write because you didn't have a stable internet connection?**
  - *Options: Yes, No*
  - Relevance: May further validate the use of a desktop application as opposed to a cloud based
    solution.
- **How concerned are you that you would lose access to your old scripts if you stopped paying for your current software?**
  - *Options: Not at all, Slightly, Moderately, Very, Extremely*
  - Relevance: Could justify the use of Open Screenplay Format and data sovereignty.
- **How often do you have to manually fix indentation or margins that the software "guessed" incorrectly?**
  - *Options: Never, Rarely, Sometimes, Often, Constantly*
  - Relevance: Justifies the need for a strict Industry Standard Engine.
- **How satisfied are you with the time it takes from opening the application to being ready to write?**
  - *Options: Not at all, Slightly, Moderately, Very, Extremely*
  - Relevance: Informs technical requirements for a resource efficient application.

### Feature Prioritisation

This portion of the survey will be rating how important different features are from:
    - *Not at all > Slightly > Moderately > Very > Extremely*

Along with a prompt at the end of the section asking if there are any features not mentioned in the 
section that the user prioritises.

This results from this section directly informs the feature prioritisation part of my project.

#### Writing Engine

- **SmartType**: Automatic completion of Character names and Scene Headings.
- **Focus Mode**: Hiding all UI elements except the page being typed on.
- **View Modes**: How pages are viewed with page breaks. Normal Mode, Page Mode, Continuous Mode. 
- **Bookmarks**: Linked to an area in the script for easy access.
- **Customisable Key Bindings**: Setting shortcut keybindings.
- **Multiple window views**: The ability to view two or more window views in appliaction e.g. Script
  and Beatmap
- **Toggleable Alternate Dialogue Lines**: Setting alternate dialogue lines that can be toggled 
  between.
- **Stash Feature**: A safe scrapbook area to cut, copy, paste parts of a script without changing 
  the main script.

#### Story Planning & Architecture

- **Scene Navigator**: A sidebar that lists acts and scenes for reordering and quick jumping.
- **Beat Map**: An area for creating and viewing beats.
- **Outline Editor**: A chronological bar showing page count along with page duration of acts, 
  scenes, beats.
- **Templates**: Various Templates for different uses of a script (Screenplay, Stageplay, TV, Short 
  Film, etc.).
- **Binder**: File organisation for extra files such as research.
- **Index Cards**: Viewing Scenes sequentially with editing functionality for summaries and order.
- **Storyline visualisation**: Color Coded beat/scene timeline showing a heatmap of beats/scenes 
  relating to plot points.

#### Production & Workflow

- **Revision Tracking**: The ability to see revised pages and lines with colour coding.
- **Script Notes/Comments**: Adding notes/comments that do not appear in an exported PDF.
- **Tagging**: Tagging props, characters, locations, etc. that can later be used for production 
  reports.
- **Further Production Features**: Catalog, budgeting, Scheduling for shoot, call sheets
- **Script Reports**: e.g. Showing statistics for characters (number of lines, number of scenes, 
  all locations)
- **Labels and Status**: Metadata for scenes and documents detailing a status (to do, in progress, 
  needs revision) and user defined labels.

#### Collaboration & Data Management

- **Collaboration**: The ability to collaborate with other writers.
- **Snapshots**: Backup copies of the script at moment of snapshot that can be reverted back to.
- **Branch Copies**: The ability to create experimental versions of a script and later merge 
  specific changes back into the master copy.

#### Accessibility & Value

- **Dark Mode**: A UI setting for writing in low-light environments.
- **Tutorials**: Educational elements for each feature of the application.
- **Productivity Tools**: Sprint Timer, Word Goals etc.

### Technical Preferences
The data gathered from this part of the survey will have a direct effect on the finalised
requirements for the project including feature prioritisation.

- **If forced to choose, would you prefer your data on a cloud server or your local hard drive?**
  - *Options: Cloud Server, Local Hard Drive*
  - Relevance: Informs the data architecture requirement.
- **How much does the availability of 'Open Source' code impact your trust in a software product?**
  - *Options: Not at all, Slightly, Moderately, Very, Extremely*
  - Relevance: Informs the use of MIT License and measures "Open Source" as a technical detail or
    a marketing advantage.
- **How Important is total data privacy (where the developer has no access to your files)?**
  - *Options: Not at all, Slightly, Moderately, Very, Extremely*
  - Relevance: May validate a push for data privacy.
- **Do you prefer a dedicated Desktop App or a Browser based tool and why?**
  - *Options: Desktop App, Browser Based* **with text field**
  - Relevance: Justifies the platform of choice.
- **How critical is it that you can open `.fdx` files?**
  - *Options: Not at all, Slightly, Moderately, Very, Extremely*
  - Relevance: Informs a critical functional requirement.
- **Do you prefer auto-updates or manual updates?**
  - *Options: Auto-Updates, Manual Updates*
  - Relevance: Informs the project management and deployment aspects of the project.
- **How much do you value an application that is "lightweight" and preserves laptop battery life?**
  - *Options: Not at all, Slightly, Moderately, Very, Extremely*
  - Relevance: May justify the use of a more memory efficient framework.
- **How important is it that the file format used is a "universal format (like XML/Fountain) rather than a secret one?**
  - *Options: Not at all, Slightly, Moderately, Very, Extremely*
  - Relevance: Validates the technical research on data handling.
- **Would you be happy  with software sending "Usage Data" back to the developers to "improve the experience"?**
  - *Options: Yes, No*
  - Relevance: Addresses Ethics and GDPR.


## Notes

- The features list is very long and I want to figure out/address the problem of survey fatigue.
