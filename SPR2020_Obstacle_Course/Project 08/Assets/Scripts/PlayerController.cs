using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class PlayerController : MonoBehaviour
{
    /* --------------------------------------------------------------
        PLAYER VARIABLES
    --------------------------------------------------------------- */
    // Movement variables
    public bool isSprint;
    public float playerSpeed;
    public float playerNormalSpeed;
    public float playerSprintSpeed;

    // Jump variables
    public float jumpForce;
    public float normalJumpForce;
    public float sprintJumpForce;
    public Vector3 jumpDir;
    public Vector3 playerGravity;
    public bool isGrounded;
    public Rigidbody rb;

    /* --------------------------------------------------------------
        INSTANTIATE VARIABLES
    --------------------------------------------------------------- */
    void Start()
    {
        playerSpeed = playerNormalSpeed;
        // Instantiate components
        rb = gameObject.GetComponent<Rigidbody>();

        // Instantiate jump direction 
        jumpDir = Vector3.up;
        
    }

    /* --------------------------------------------------------------
        UPDATE
    --------------------------------------------------------------- */
    void Update()
    {
        // Gravity
        if(!isGrounded) {
            rb.AddForce(playerGravity);
        }

        // Check what the player's movement speed and jump force is
        if(Input.GetKey(KeyCode.LeftShift)) {
            playerSpeed = playerSprintSpeed;
            jumpForce = sprintJumpForce;
            isSprint = true;
        }
        else {
            playerSpeed = playerNormalSpeed;
            jumpForce = normalJumpForce;
            isSprint = false;
        }

        // Generic jump
        if(Input.GetKey(KeyCode.UpArrow) && isGrounded) {
            rb.AddForce(jumpDir * jumpForce, ForceMode.Impulse);
            isGrounded = false;
        }

        // Left-right arrow key movement, x-axis
        Vector3 moveInput = new Vector3(Input.GetAxisRaw("Horizontal"), 0, 0);
        transform.position += moveInput * Time.deltaTime * playerSpeed;
        
    }

    void FixedUpdate() {
        
    }

    /* --------------------------------------------------------------
        COLLISION
    --------------------------------------------------------------- */
    void OnCollisionEnter(Collision other) {
        // Check if the player is grounded or not.
        if(other.gameObject.tag == "floor") {
            isGrounded = true; 
        }           
    }

    void OnCollisionExit(Collision other) {
        // Deactivate grounded state.
        if(other.gameObject.tag == "floor") {
            isGrounded = false;
        }
    }

    /* --------------------------------------------------------------
        HELPER FUNCTIONS
    --------------------------------------------------------------- */
    public Vector3 FindClosestBlock()
    {
        GameObject[] gos;
        gos = GameObject.FindGameObjectsWithTag("floor");
        GameObject closest = null;
        float distance = Mathf.Infinity;
        Vector3 position = transform.position;
        foreach (GameObject go in gos)
        {
            Vector3 diff = go.transform.position - position;
            float curDistance = diff.sqrMagnitude;
            if (curDistance < distance)
            {
                closest = go;
                distance = curDistance;
            }
        }

        Collider coll = closest.GetComponent<Collider>();
        Vector3 closestPoint = coll.ClosestPoint(transform.position);
        return closestPoint;
    }

    public bool isSprinting() {
        return isSprint;
    }
}
