using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class ObstacleSpawner : MonoBehaviour
{
    /* ---------------------------------------------------------------------
        Dumb spawner object that just pops out a new object every so often.
    ------------------------------------------------------------------------ */
    /* ---------------------------------------------------------------------
        SPAWNER VARIABLES
    ------------------------------------------------------------------------ */
    public GameObject spawnObject;
    public GameObject target;

    // How long it takes to spawn an object (low value --> fast spawns, high value --> slow spawns)
    public float spawnTime;
    public float spawnTimeCounter;
    public bool isSpawning;

    // Area where objects can be spawned
    /*  xMin, yMax  -----------------------------
                    |                           |
                    ----------------------------- xMax, yMin
    */
    public float spawnBoundaryXMin;
    public float spawnBoundaryXMax;
    public float spawnBoundaryYMin;
    public float spawnBoundaryYMax;

    public float launchForce;
    public float launchForceUp;

    /* ---------------------------------------------------------------------
        INSTANTIATE VARIABLES
    ------------------------------------------------------------------------ */
    void Start()
    {
        // Setting the counter
        spawnTimeCounter = spawnTime;
        target = GameObject.FindWithTag("Player");
        isSpawning = false;
    }

    /* ---------------------------------------------------------------------
        UPDATE
    ------------------------------------------------------------------------ */
    void Update()
    {
        // Decrement counter to spawn
        spawnTimeCounter -= Time.deltaTime;
        if(spawnTimeCounter <= 0.0f && isSpawning) {
            // Get a random position to spawn an enemy object
            Vector3 randomPosition = GetRandomPosition();
            // Spawn an object
            LaunchOneEnemy(randomPosition);
            // Reset the counter
            spawnTimeCounter = spawnTime;
        }
    }

    /* -----------------------------------------------------------
        COLLISION DETECTION
    -------------------------------------------------------------- */
    void OnTriggerEnter(Collider other) {
        if(other.gameObject.tag == "Player") {
            isSpawning = true;
        }
    }

    void OnTriggerExit(Collider other) {
        if(other.gameObject.tag == "Player") {
            isSpawning = false;
        }
    }
    /* -----------------------------------------------------------
        SPAWN HELPER FUNCTIONS
    -------------------------------------------------------------- */
    public void SpawnOneEnemy(Vector3 spawnPosition) {
        // Spawn an instance of the object
        GameObject obj = Instantiate(spawnObject, spawnPosition, Quaternion.identity);       
    }

    public void LaunchOneEnemy(Vector3 spawnPosition) {
        // Spawn an instance of the object
        GameObject obj = Instantiate(spawnObject, spawnPosition, Quaternion.identity);       
        // Launch object
        Vector3 direction = target.transform.position - obj.transform.position;
        obj.GetComponent<Rigidbody>().AddForce(direction * launchForce, ForceMode.Impulse);
        obj.GetComponent<Rigidbody>().AddForce(Vector3.up * launchForceUp, ForceMode.Impulse);
    }

    Vector3 GetRandomPosition() {
        // Get a random position somewhere in the map
        Vector3 randomPosition = new Vector3(Random.Range(spawnBoundaryXMin, spawnBoundaryXMax), Random.Range(spawnBoundaryYMin, spawnBoundaryYMax), transform.position.z); 
        return randomPosition;
    } 
}
