CS 210 PA2: Story Tree Starter

What is in here
  main.cpp        PART 0 prints your story file. PART 1 (commented) is the game menu.
  Adventure.h     The game class. Do not change.
  Adventure.cpp   Given helpers at the top. TODO 10 to 19 below them.
  Scene.h         One scene. TODO 1 to 5.
  StoryTree.h     The whole story. TODO 6 to 9.
  List.h, Node.h  A working linked list for the inventory. Do not change.
  Moment.h        What one scene holds. Do not change.
  Item.h          One inventory item. Do not change.
  bee_movie.txt   The sample story.
  bad_story.txt   A broken story. Your program must reject it.
  my_story.txt    Replace this with YOUR story.
  CMakeLists.txt  Builds the program "adventure" and copies the .txt files next to it.

First run
  CLion: Run > Edit Configurations > Program arguments: bee_movie.txt
  Terminal: ./adventure bee_movie.txt
  You should see the story file printed line by line.

Then
  Read the assignment PDF. Work the TODOs in order: Scene.h, StoryTree.h, Adventure.cpp.
  When it all compiles, comment out PART 0 in main.cpp and uncomment PART 1.

If you edit a .txt story file, rebuild so the new copy goes next to the program.
