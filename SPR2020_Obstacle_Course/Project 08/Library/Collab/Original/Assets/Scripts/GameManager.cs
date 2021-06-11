using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.SceneManagement;

public class GameManager : MonoBehaviour
{
    /* --------------------------------------------------------------
        VARIABLES
    --------------------------------------------------------------- */
    // Game state
    public bool isPaused;

    // Player state info
    public GameObject player;
    public int lastCheckpointID;
    public Vector3 lastCheckpointPos;

    // Environment info
    public GameObject[] cylinderStacks;

    // Call this variable in external scripts to access manager functions
    public static GameManager gameManager;


    /* --------------------------------------------------------------
        INSTANTIATE VARIABLES
    --------------------------------------------------------------- */
    private void Awake() {
        gameManager = this;
    }

    void Start()
    {
        isPaused = false;
        player = GameObject.FindGameObjectWithTag("Player");
        lastCheckpointID = 0;
        if(lastCheckpointPos == Vector3.zero) {
            // Default checkpoint position
            lastCheckpointPos = player.transform.position;
        }

        cylinderStacks = GameObject.FindGameObjectsWithTag("floor");
    }

    /* --------------------------------------------------------------
        UPDATE
    --------------------------------------------------------------- */
    void Update()
    {
        // Restart game
        if(Input.GetKeyDown(KeyCode.R)) {
            //RestartGame();
            SceneManager.LoadScene(SceneManager.GetActiveScene().name);
        }
        
    }

    /* --------------------------------------------------------------
        HELPER FUNCTIONS (can be called by external scripts)
    --------------------------------------------------------------- */
    public void RestartGame() {
        // Reset any of the player's rigidbody velocity
        player.gameObject.GetComponent<Rigidbody>().velocity = new Vector3(0f,0f,0f); 
        player.gameObject.GetComponent<Rigidbody>().angularVelocity = new Vector3(0f,0f,0f);
        player.gameObject.GetComponent<Rigidbody>().rotation = Quaternion.Euler(new Vector3(0f,0f,0f));
        // Drop off the player at the most recent checkpoint
        player.transform.position = lastCheckpointPos;

        // Reset the environment
        for(int i = 0; i < cylinderStacks.Length; i++) {
            if(cylinderStacks[i].GetComponent<ReturnPosition>() != null) {
                cylinderStacks[i].GetComponent<ReturnPosition>().ResetPosition();
            }
            
        }
    }

    public int GetCurrentCheckpoint() {
        return lastCheckpointID;
    }

    public void SetCurrentCheckpoint(int c, Vector3 p) {
        lastCheckpointID = c;
        lastCheckpointPos = p;
    }
}
