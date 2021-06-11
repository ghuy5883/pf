using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class EllipticalStairs : MonoBehaviour
{
    /* --------------------------------------------------------------
        ELLIPTICAL VARIABLES
    --------------------------------------------------------------- */
    // Elliptical movement or boxy movement
    public int movementType; // -- 0 for boxy movement, 1 for elliptical

    // Boxy movement variables -------------------------------------
    public float minX;
    public float maxX;
    public float minY;
    public float maxY;
    public float minZ;
    public float maxZ;
    private Vector3 startPos;
    private Vector3 destPos;
    public int destPosID;

    // Elliptical movement variables -------------------------------
    // Origin of movement
    public float originX;
    public float originY;
    public float originZ;

    // Radius along a given axis
    public float radiusX;
    public float radiusY;
    public float radiusZ;

    // Angle of rotation
    public float angle;

    // Determines which axis the movement happens along
    public int rotationAxis; // -- 0 for x-axis, 1 for y-axis, 2 for z-axis

    // Speed of rotation
    public float rotationSpeed;
    public float flipSpeed;
    public float flipAngle;
    /* --------------------------------------------------------------
        INSTANTIATE VARIABLES
    --------------------------------------------------------------- */
    void Start()
    {
        // Give the object default values
        if(originX == 0) {
            originX = transform.position.x;
        }
        if(originY == 0) {
            originY = transform.position.y;
        }
        if(originZ == 0) {
            originY = transform.position.z;
        }
        if(radiusX == 0) {
            radiusX = 5.0f;
        }
        if(radiusY == 0) {
            radiusY = 5.0f;
        }
        if(radiusZ == 0) {
            radiusZ = 5.0f;
        }
        if(rotationSpeed == 0) {
            rotationSpeed = 1.0f;
        }

        if(rotationAxis == 0) {
            startPos = new Vector3(transform.position.x, minY, minZ);
            if(destPosID == 0) {
                destPos = new Vector3(gameObject.transform.position.x, minY, minZ);
            }
            else if(destPosID == 1) {
                destPos = new Vector3(gameObject.transform.position.x, minY, maxZ);
            }
            else if(destPosID == 2) {
                destPos = new Vector3(gameObject.transform.position.x, maxY, maxZ);
            }
            else if(destPosID == 3) {
                destPos = new Vector3(gameObject.transform.position.x, maxY, minZ);
            }
        }
    }

    /* --------------------------------------------------------------
        UPDATE
    --------------------------------------------------------------- */
    
    void Update()
    {
        if(movementType == 0) {
            if(rotationAxis == 0) {
                // Move in a straight line towards the next point
                Vector2 temp = Vector2.MoveTowards(new Vector2(gameObject.transform.position.y, gameObject.transform.position.z), new Vector2(destPos.y, destPos.z), rotationSpeed * Time.deltaTime);
                Vector3 moveVector = new Vector3(gameObject.transform.position.x, temp.x, temp.y);
                transform.position = moveVector;

                if(transform.position == destPos) {
                    // Change destination
                    ChangeDestination();
                    flipAngle -= 90;
                }
            }

            transform.rotation = Quaternion.Slerp(transform.rotation, Quaternion.Euler(flipAngle, 0, 0), flipSpeed * Time.deltaTime);
        }
        else if(movementType == 1) {
            angle += Time.deltaTime;
            if(angle >= Mathf.PI) {
                angle = 0.0f;
            }
            
            if(rotationAxis == 0) {
                // Rotate along the x-axis
                RotateAlongX();

            }
            else if(rotationAxis == 1) {
                // Rotate along the y-axis
                RotateAlongY();

            }
            else if(rotationAxis == 2) {
                // Rotate along the z-axis
                RotateAlongZ();
            }
        } 
    }

    /* --------------------------------------------------------------
        COLLISION DETECTION
    --------------------------------------------------------------- */

    void OnCollisionStay(Collision other){
        if(other.gameObject.tag == "Player"){
            // Implements "moving platform" behavior
            other.transform.parent = transform;
 
        }
     }
 
    void OnCollisionExit(Collision other){
     if(other.gameObject.tag == "Player"){
            other.transform.parent = null;
             
         }
     }

    /* --------------------------------------------------------------
        HELPER FUNCTIONS
    --------------------------------------------------------------- */
    public void RotateAlongX() {
        float positionZ = originZ + (radiusZ * Mathf.Cos(angle * rotationSpeed));
        float positionY = originY + (radiusY * Mathf.Sin(angle * rotationSpeed));

        gameObject.transform.position = new Vector3(originX, positionY, positionZ);

        // Flip the step down as it nears the 3PI/2 (270 deg)
        if(angle >= ((2.0f/3.0f)*Mathf.PI) && angle <= Mathf.PI) {
            // Deactivate motor to flip step down
            //gameObject.GetComponent<HingeJoint>().useMotor = false;
        }
        else {
            //gameObject.GetComponent<HingeJoint>().useMotor = true;
        }

    }
    public void RotateAlongY() {
        float positionZ = originZ + (radiusZ * Mathf.Sin(angle * rotationSpeed));
        float positionX = originX + (radiusX * Mathf.Cos(angle * rotationSpeed));

        gameObject.transform.position = new Vector3(positionX, originY, positionZ);
    }
    public void RotateAlongZ() {
        float positionX = originX + (radiusX * Mathf.Cos(angle * rotationSpeed));
        float positionY = originY + (radiusY * Mathf.Sin(angle * rotationSpeed));

        gameObject.transform.position = new Vector3(positionX, positionY, originZ);
    }

    private void ChangeDestination() {
        if(destPosID == 0) {
            destPos = new Vector3(gameObject.transform.position.x, minY, maxZ);
            destPosID = 1;
        }
        else if(destPosID == 1) {
            destPos = new Vector3(gameObject.transform.position.x, maxY, maxZ);
            destPosID = 2;
        }
        else if(destPosID == 2) {
            destPos = new Vector3(gameObject.transform.position.x, maxY, minZ);
            destPosID = 3;   
        }
        else if(destPosID == 3) {
            destPos = new Vector3(gameObject.transform.position.x, minY, minZ);
            destPosID = 0;
        }
    }
}
