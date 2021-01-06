# Centipede-Game-and-Engine-Snippets

Eric Hollas

Here is a breakdown of the files posted. The total project is over 100 files long.

Here is posted:
1) The memory allocator
2) The definition of the BaseCharacter class and a glimpse into how the characters are managed and interface with the engine
3) A sample of a Character, the Centipede, with a look into the state-machine pattern used
4) A description of the variuos components of the game engine

Note when looking into code:
I defined and used my own API defined in a namespace Master. So in code you will notice several uses of Master::Math and Master::Physics. There Math, physics, error, and memory libraries in this API. The memory allocator defined here is a part of this Master API I am developing for more sophisticated engines in the future.

&&&&&&&&&&&&&&&&&&&&&&&&&&
TOPIC 1 : Memory Allocator

A rather simple memory allocator. A list of freed and/or unsued blocks of memory.
When memory runs out more is requested. None of the planned projects will experience memory shortfalls.
Each use of this allocator will be defined statically in an object. Thus each type should have a memory allocator. However, this was added late in the process of this project, so that is not entirely true for all types.

&&&&&&&&&&&&&&&&&&&&&&&&&&
TOPIC 2 : BaseCharacter

The character class used to define the information the game engine requires of each character, and is the base class of all renderable characters in this game.

After completing this project it is obvious to me to fully utiliize polymorphism in the BaseCharacter class as virtual pointers in managing data structures, it would be important to create a heirarchy of character types similar to that the heirarchy in UE4, i.e. the Pawn, the character, and the actor.

You will see in the header file for the BaseCharacter lines 30 through 43 describe the virtual functions. In addition to the handling of Message objects to signal events, also note lines 57 through 64 to see a glimpse in how this defines an interface between characters and the game engine as a whole.

&&&&&&&&&&&&&&&&&&&&&&&&&&
TOPIC 3 : Centipede

Briefly stated this character is composed from the BaseCharacter object and has member variables of type StateMachine to manage its various states.
The various states will call member functions of the centipede to change its values to give it motion.
This is true for all of the characters defined in this engine.

As for the centipede in particular, the centipede takes a structure of a doubly linked list.
Each train of centipede objects, whether it be the head or a following segment, is defined in this object class.
The states manage each objects individual movement. So to create a train of motion, the directional data is passed from one object to the next.
This simplifies the various situations when a train is shot in the middle and the following segment must branch off as the head of a new train or when the head of a train is shot and the next must become the head.

This is managed as a conceptual doubly linked list.
There is a wrapper class of enumerated data that encapsulates the directional data and is passed from segment to segment in the list.
There are member pointers in each of the Centipede that reference the follower and the followed by in the list, hence doubly linked.
If the follower is empty, then the centipede is the head of the train of centipedes.
This will mostly be used by the graphics engine to render ther various centipede objects accordingly to their character models.

&&&&&&&&&&&&&&&&&&&&&&&&&&
TOPIC 4 : Breakdown of the Game Engine

As a brief breakdown of the game engine, it uses C++, the WIN32 API, HLSL, and DirectX 12.
There is a hardward layer that initializes the window objects and the GPU objects (i.e. the command queue, the command allocator, the command list, ...etc.)
There is a memory layer that initializes the descriptors, root signatures, the shaders, and the various PSOs.
There is another layer that manages the frame resource buffers.
These compose system necessities to render the game and the Game object in large.
(The break down of this portion of the game engine is a para-phrase from a popular textbook by Frank Luna. It is designed for future iterations to handle different things like addeding new PSO, shaders, and various lighting effects.)

The game, in parallel, reads in the render data from .obj and .dds files and partitions it to the various buffers needed for rendering. 
A buffer of Texture objects is used to keep track of the various textures and is uploaded in one buffer from the CPU to the GPU.
A material buffer for the various objects is used to keep track of the indices of in the texture buffer for the various objects.
An instance buffer is used to keep track of the matrix data and material data. 
Instance rendering is used to cut down on draw calls.

There are 3 separate Pipeline State Objects used, one for the game objects, one for the UI objects (used to bypass some of the lighting effects), and another for the shadow map.
The shadow map uses its own shader to render from a light source depth values of the scene to a texture. That texture is then mapped to a descriptor, and is used as a texture in the game objects shader to sample values from and used to calculate shadow lighting.

Finally, the CharacterMaster object in the Game manages the various characters as state-machines, the various Message events between them, and the Message events inputted from the WIN32 API as user input on keystrokes.
At the beginning of each Update function within the CharacterMaster, the various characters' physics data is loaded into 3 buffers according to their physics types and collision detection is performed in 3 separate threads while the various game objects are updating. 
At the end of the Update function the various buffers of collisions are handled acordingly.
Because of the locality of the physics to the Update function in the CharacterMaster object, the physics bodies are defined as callable objects.

For further details about the breakdown of this game please feel free to ask away.

