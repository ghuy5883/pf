using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class ObstacleController : MonoBehaviour
{
    /* --------------------------------------------------------------
        OBSTACLE VARIABLES
    --------------------------------------------------------------- */
    // Type of obstacle
    public enum ObstacleType
    {
        // Does nothing
        still,
        // Single-direction movement
        ray,
        // Moves back and forth between two points, swaps state to 'chaser' if player is near
        patrol,
        // Pursues target, swaps state to 'patrol' if player goes out of range
        chaser        
    }
    public ObstacleType obstacleState;
    // Toggle whether patrol mode will chase player when in range or not
    public bool activePatroller;

    // Movement variables
    public float obstacleSpeed;
    public Vector3 destPos; // -- used for moving obstacles, can set in inspector
    public Vector3 startPos; // -- used for moving obstacles, can set in inspector
    public Rigidbody rb;
    
    // Variables for chaser state
    public GameObject target;
    public float aggroRadius; // -- larger radius means more easily aggroed

    /* --------------------------------------------------------------
        INSTANTIATE VARIABLES
    --------------------------------------------------------------- */
    void Start()
    {
        target = GameObject.FindGameObjectWithTag("Player");

        if(startPos == Vector3.zero) {
            // Default starting position of an obstacle
            startPos = transform.position;
        }
        if(destPos == Vector3.zero) {
            // Default destination of an obstacle, somewhere left of the start position
            destPos = startPos + new Vector3(-10, 0, 0);
        }
        if(obstacleSpeed == 0) {
            // Default speed
            obstacleSpeed = 5.5f;
        }
        if(aggroRadius == 0) {
            aggroRadius = 7;
        }
    }

    /* --------------------------------------------------------------
        UPDATE
    --------------------------------------------------------------- */
    void Update()
    {

        // Is player close by?
        Vector3 obstacleToPlayer = target.transform.position - transform.position;
        float distanceToPlayer = obstacleToPlayer.magnitude;
        
        switch(obstacleState) {
            case ObstacleType.still:
            // Do nothing
            break;

            case ObstacleType.ray:
            // Movement from start to destination, destination to start, destroy when destination reached
            Vector2 rayTemp = Vector2.MoveTowards(new Vector2(gameObject.transform.position.x, gameObject.transform.position.y), new Vector2(destPos.x, destPos.y), obstacleSpeed * Time.deltaTime);
            Vector3 rayMove = new Vector3(rayTemp.x, rayTemp.y, gameObject.transform.position.z);
            transform.position = rayMove;

            if(transform.position == destPos) {
                Destroy(gameObject);
            }
            break;

            case ObstacleType.patrol:
            if(distanceToPlayer < aggroRadius && activePatroller) {
                // If so, chase player
                obstacleState = ObstacleType.chaser;
                break;
            }

            // Movement from start to destination, destination to start
            Vector2 temp = Vector2.MoveTowards(new Vector2(gameObject.transform.position.x, gameObject.transform.position.y), new Vector2(destPos.x, destPos.y), obstacleSpeed * Time.deltaTime);
            Vector3 moveVector = new Vector3(temp.x, temp.y, gameObject.transform.position.z);
            transform.position = moveVector;

            // If the enemy reaches its destination, reverse the movement
            if(transform.position == destPos) {
                // Swap the dest and start vectors
                Vector3 swap = startPos;
                startPos = destPos;
                destPos = swap;
            }
            break;

            case ObstacleType.chaser:
            if(distanceToPlayer < aggroRadius) {
                // Chase player
                Vector2 chaseTemp = Vector2.MoveTowards(new Vector2(gameObject.transform.position.x, gameObject.transform.position.y), new Vector2(target.transform.position.x, target.transform.position.y), obstacleSpeed * Time.deltaTime);
                Vector3 chaseVector = new Vector3(chaseTemp.x, chaseTemp.y, gameObject.transform.position.z);

                transform.position = chaseVector;
            }
            else {
                // If player is out of range, go back to patrolling mode
                obstacleState = ObstacleType.patrol;
            }
            break;

        }
        
    }

    /* --------------------------------------------------------------
        COLLISION DETECTION
    --------------------------------------------------------------- */
    void OnTriggerEnter(Collider other) {
        // If contact with player, reset the level
        if(other.tag == "Player") {
            GameManager.gameManager.RestartGame();
        }
    }

    /* --------------------------------------------------------------
        HELPER FUNCTIONS
    --------------------------------------------------------------- */
    public void SetState(int i) {
        if (i == 0) {
            obstacleState = ObstacleType.still;
        }
        else if( i== 1) {
            obstacleState = ObstacleType.ray;
        }
        else if( i == 2) {
            obstacleState = ObstacleType.patrol;
        }
        else if (i == 3) {
            obstacleState = ObstacleType.chaser;
        }
    }

    public void SetDestPos(Vector3 d) {
        destPos = d;
    }

    public void SetStartPos(Vector3 s) {
        startPos = s;
    }

    public void SetSpeed(float s) {
        obstacleSpeed = s;
    }
}
