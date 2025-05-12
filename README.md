"funny game library"
-me 

i don't know what are my goals with this thing buuuuut

Event module -> (Done)

Window module -> (Done)

    - Poll based input -> (Done)

    - Event based input -> (Done)

    - Actual window -> (Done)

    - Error logging -> (Done)
    
    - Common callbacks -> (Done)

Renderer module -> (WIP)

    - Basic pipeline -> (Done)

    - 2D -> (WIP)
        - Simple shape rendering -> (Done)(hahghgaha funny regtancle)
        - Textures -> (WIP)

    3D -> (WIP)

        - Simple shape rendering -> (WIP)
        - Mesh loading -> (WIP)
        - Textures -> (WIP)

Audio module -> (WIP)

    - Basic music playing shit (play a wav file) -> (On it rn)
        - Load WAV files (On it rn)
        - Play basic audio (WIP)

    - Event integration (everything should have an event_system,
        and when a sound does a specific action it fires the 
        corresponding event, like if the sound just started playing
        it will fire event_system with an event
        that event->type == AUDEVENT_SOUND_PLAYED or something)
        
    - 3D stuff (no plans cause idk nothing bout it) -> (WIP)


ECS module -> (WIP) (no plans yet)

UI -> (WIP) (no plans yet) 

for contribution, read docs/STYLE or something idk but it's def in docs
