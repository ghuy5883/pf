using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.SceneManagement;
using UnityEngine.UI;

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

    private Checkpoint[] points;
    private GameObject[] floorObj;
    

    // Call this variable in external scripts to access manager functions
    public static GameManager gameManager;

    // Timer UI
    public float timeCounter;
    public Text timeText;
    public bool isTiming;

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

        points = (Checkpoint[])GameObject.FindObjectsOfType(typeof(Checkpoint));
        //Light[] lights = (Light[])GameObject.FindObjectsOfType(typeof(Light));
        floorObj = GameObject.FindGameObjectsWithTag("floor");

        timeCounter = 0.0f;
        isTiming = true;
    }

    /* --------------------------------------------------------------
        UPDATE
    --------------------------------------------------------------- */
    void Update()
    {
        // Update time
        if(isTiming){ 
            timeCounter += Time.deltaTime;
            double time = (Mathf.Round(timeCounter * 100)) / 100.0;
            timeText.text = time.ToString();

        }

        // Restart game
        if(Input.GetKeyDown(KeyCode.R)) {
            //RestartGame();
            SceneManager.LoadScene(SceneManager.GetActiveScene().name);
        }

        if (Input.GetKeyDown(KeyCode.Alpha1))
        {
            Vector3 pos = Vector3.zero ;
            for (int i=0; i<points.Length; i++)
            {
                if (points[i].checkpointID == 1)
                {
                    pos = points[i].gameObject.transform.position;
                    SetCurrentCheckpoint(1, pos);
                    RestartGame();
                    break;
                }
            }
            
        }
        if (Input.GetKeyDown(KeyCode.Alpha2))
        {
            Vector3 pos = Vector3.zero;
            for (int i = 0; i < points.Length; i++)
            {
                if (points[i].checkpointID == 2)
                {
                    pos = points[i].gameObject.transform.position;
                    SetCurrentCheckpoint(2, pos);
                    RestartGame();
                    break;
                }
            }

        }
        if (Input.GetKeyDown(KeyCode.Alpha3))
        {
            Vector3 pos = Vector3.zero;
            for (int i = 0; i < points.Length; i++)
            {
                if (points[i].checkpointID == 3)
                {
                    pos = points[i].gameObject.transform.position;
                    SetCurrentCheckpoint(3, pos);
                    RestartGame();
                    break;
                }
            }

        }
        if (Input.GetKeyDown(KeyCode.Alpha4))
        {
            Vector3 pos = Vector3.zero;
            for (int i = 0; i < points.Length; i++)
            {
                if (points[i].checkpointID == 4)
                {
                    pos = points[i].gameObject.transform.position;
                    SetCurrentCheckpoint(4, pos);
                    RestartGame();
                    break;
                }
            }

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
        for(int i = 0; i < floorObj.Length; i++) {
            if(floorObj[i].GetComponent<ReturnPosition>() != null) {
                floorObj[i].GetComponent<ReturnPosition>().ResetPosition();
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

    public void SetIsTiming(bool t) {
        isTiming = t;
    }
}
