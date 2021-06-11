using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class Checkpoint : MonoBehaviour
{
    /* --------------------------------------------------------------
        CHECKPOINT VARIABLES
    --------------------------------------------------------------- */
    public Vector3 location;
    public int checkpointID; // -- Higher value means further/most recent progress (need to change value in the inspector for each new checkpoint)
    public bool isFinalCheckpoint;
    /* --------------------------------------------------------------
        INSTANTIATE VARIABLES
    --------------------------------------------------------------- */
    void Start()
    {
        if(location == Vector3.zero) {
            // Default checkpoint position
            location = transform.position; 
        }

    }

    /* --------------------------------------------------------------
        UPDATE
    --------------------------------------------------------------- */
    void Update()
    {
        
    }

    /* --------------------------------------------------------------
        COLLISION DETECTION
    --------------------------------------------------------------- */
    void OnTriggerStay(Collider other) {
        if(other.tag == "Player") {

            // Check if this point is further ahead than currently set player checkpoint
           int mostRecentCheckpoint = GameManager.gameManager.GetComponent<GameManager>().GetCurrentCheckpoint();
           if(mostRecentCheckpoint < checkpointID) {
               // Set as most current checkpoint
               GameManager.gameManager.GetComponent<GameManager>().SetCurrentCheckpoint(checkpointID, location);
           }

           if(isFinalCheckpoint) {
               GameManager.gameManager.GetComponent<GameManager>().SetIsTiming(false);
           }
        }
    }

    /* --------------------------------------------------------------
        HELPER FUNCTIONS
    --------------------------------------------------------------- */
}
    