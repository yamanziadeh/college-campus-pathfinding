<!-- PROJECT SHIELDS -->
<!--
*** I'm using markdown "reference style" links for readability.
*** Reference links are enclosed in brackets [ ] instead of parentheses ( ).
*** See the bottom of this document for the declaration of the reference variables
*** for contributors-url, forks-url, etc. This is an optional, concise syntax you may use.
*** https://www.markdownguide.org/basic-syntax/#reference-style-links
-->
[![Issues][issues-shield]][issues-url]
[![MIT License][license-shield]][license-url]
[![LinkedIn][linkedin-shield]][linkedin-url]



<!-- PROJECT LOGO -->
<br />
<p align="center">
  <h3 align="center">College Campus Pathfinding Program</h3>

  <p align="center">
A C++ Program that takes in a .osm file of any College Campus and finds the fastest possible path between Campus Buildings using many popular Pathfinding     Algorithms (A*, Dijkstra, BFS, etc.). Subsequently, it uses the Google Maps Static API to display the optimal path.
    <br />
    <a href="https://github.com/yamanziadeh/college-campus-pathfinding/"><strong>Explore the docs »</strong></a>
    <br />
    <br />
    <a href="https://github.com/yamanziadeh/college-campus-pathfinding/issues">Report Bug</a>
    ·
    <a href="https://github.com/yamanziadeh/college-campus-pathfinding/issues">Request Feature</a>
  </p>
</p>



<!-- TABLE OF CONTENTS -->
## Table of Contents

* [Getting Started](#getting-started)
  * [Installation](#installation)
* [Usage](#usage)
  * [Usage Tips](#Usage-Tips)
* [Contributing](#contributing)
* [Contact](#contact)
* [Acknowledgements](#acknowledgements)



<!-- GETTING STARTED -->
## Getting Started

To get a local copy up and running follow these simple steps.

### Installation
 
1. Clone the repo
```sh
git clone https://github.com/yamanziadeh/college-campus-pathfinding.git
```
2. Run Makefile command to get executable file
```sh
make
```



<!-- USAGE EXAMPLES -->
## Usage

Use this space to show useful examples of how a project can be used. Additional screenshots, code examples and demos work well in this space. You may also link to more resources.

1. Run the following command to run the executable
```sh
make run
```

2. You will be prompted to enter the filename, I have provided several files of various known campuses in the osmFiles folder, simply write the name of the
file (no need to include the folder name)
```
** Navigating College Open Street Map **

Enter map filename> harvard.osm
```

3. Upon successful parsing of the inputted filename, you will be given a list of the buildings and available algorithms, input the algorithm number
```
-------------------- Buildings ---------------------
...
Clinical Wing
Berkman Center for Internet & Society
Harvard Extension School
----------------------------------------------------

# of vertices: 10358
# of edges: 5612

Algorithms: 
 0) Dijkstras
 1) A-Star
 2) Breadth First Search
 3) Depth First Search

Enter Algorithm Number: 1
```

4. Input the starting and destination building names (full or partial)
```
Enter start (partial name or abbreviation), or #> Clinical
Enter destination (partial name or abbreviation)> Harvard Extension

Starting Point:
 Clinical Wing
 (42.379682, -71.119361)
Destination Point:
 Harvard Extension School
 (42.374465, -71.12148)

Nearest start node:
 7625819507
 (42.379883, -71.119135)
Nearest destination node:
 7634516229
 (42.374442, -71.121778)
 
Navigating with A-Star...
Runtime of Algorithm (microseconds): 3149
Visual Path (search up the following URL): 
```
5. After this, you will be given a URL to an image of the visual path generated using the Google Maps Static API

![Visual Path between Clinical Wing and Harvard Extension School](https://i.ibb.co/RQxKFYN/staticmap.png)



<!-- USAGE EXAMPLES -->
## Usage Tips
* If you want to upload a college campus file, go to https://www.openstreetmap.org/, export the desired map/ area, put exported .osm file into osmFiles folder.

<!-- CONTRIBUTING -->
## Contributing

Contributions are what make the open source community such an amazing place to be learn, inspire, and create. Any contributions you make are **greatly appreciated**.

1. Fork the Project
2. Create your Feature Branch (`git checkout -b feature/AmazingFeature`)
3. Commit your Changes (`git commit -m 'Add some AmazingFeature'`)
4. Push to the Branch (`git push origin feature/AmazingFeature`)
5. Open a Pull Request



<!-- CONTACT -->
## Contact

Yaman Ziadeh - yamanbader@gmail.com

Project Link: [https://github.com/yamanziadeh/college-campus-pathfinding](https://github.com/yamanziadeh/college-campus-pathfinding)



<!-- ACKNOWLEDGEMENTS -->
## Acknowledgements

* [Prof. Joseph Hummel](https://cs.uic.edu/profiles/joe-hummel/)





<!-- MARKDOWN LINKS & IMAGES -->
<!-- https://www.markdownguide.org/basic-syntax/#reference-style-links -->
[issues-shield]: https://img.shields.io/github/issues/othneildrew/Best-README-Template.svg?style=flat-square
[issues-url]: https://github.com/yamanziadeh/college-campus-pathfinding/issues
[license-shield]: https://img.shields.io/github/license/othneildrew/Best-README-Template.svg?style=flat-square
[license-url]: https://github.com/othneildrew/Best-README-Template/blob/master/LICENSE.txt
[linkedin-shield]: https://img.shields.io/badge/-LinkedIn-black.svg?style=flat-square&logo=linkedin&colorB=555
[linkedin-url]: https://www.linkedin.com/in/yaman-ziadeh/
[product-screenshot]: images/screenshot.png
