using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.SceneManagement;

public class Player : MonoBehaviour
{
    // Start is called before the first frame update

    float moveSpeed = 10.0f;
    float jumpSpeed = 17.0f;

    private Rigidbody rigid;
    private bool inAir = false;

    public GameObject splash;
    private GameObject spawned;
    public AudioClip waterSplash;
    private AudioSource audioSource;
    public AudioClip[] hitSounds;
    public AudioClip walkSound;
    public AudioClip bounceSound;


    void Start()
    {
        rigid = GetComponent<Rigidbody>();
        audioSource = GetComponent<AudioSource>();
    }

    // Update is called once per frame
    void Update()
    {
        if (Input.GetKey(KeyCode.LeftArrow))
        {
            float yVel = rigid.velocity.y;
            Vector3 newVel = new Vector3(-moveSpeed, yVel, 0.0f);
            rigid.velocity = newVel;
        }

        else if (Input.GetKey(KeyCode.RightArrow))
        {
            float yVel = rigid.velocity.y;
            Vector3 newVel = new Vector3(moveSpeed, yVel, 0.0f);
            rigid.velocity = newVel;
        }

        if (Input.GetKeyDown(KeyCode.UpArrow))
        {
            Debug.Log(inAir);
            if (!inAir)
            {
                rigid.velocity += new Vector3(0.0f, jumpSpeed, 0.0f);
                inAir = true;
            }
        }
    }

    IEnumerator WaitBeforeReload()
    {
        yield return new WaitForSeconds(2);
        //SceneManager.LoadScene(SceneManager.GetActiveScene().name);
        GameManager.gameManager.GetComponent<GameManager>().RestartGame();
        Destroy(spawned);
    }

    private void OnTriggerEnter(Collider other)
    {
        if (other.gameObject.tag == "death")
        {
            Debug.Log("you lose");
            Vector3 spawnPos = new Vector3(transform.position.x, transform.position.y + 5, transform.position.z);
            spawned = Instantiate(splash, transform.position, Quaternion.identity);
            spawned.transform.localScale *= 5;
            audioSource.clip = waterSplash;
            audioSource.volume = 1.0f;
            audioSource.Play();
            StartCoroutine(WaitBeforeReload());
        }
        else if (other.gameObject.tag == "bounceball")
        {
            audioSource.clip = bounceSound;
            audioSource.volume = 1.0f;
            audioSource.Play();
        }
    }


    private void OnCollisionEnter(Collision collision)
    {
       
        
        //Debug.Log("collision");
        if (collision.gameObject.tag == "floor")
        {
            inAir = false;
            audioSource.volume = 0.5f;
            audioSource.clip = walkSound;
            audioSource.Play();
        }

        else
        {
            int rand = Random.Range(0, hitSounds.Length - 1);
            audioSource.volume = 1.0f;
            audioSource.clip = hitSounds[rand];
            audioSource.Play();
        }
        
        if (collision.gameObject.tag == "enemy")
        {
            //Debug.Log("you lose");
            //SceneManager.LoadScene(SceneManager.GetActiveScene().name);

        }

        
    }
}
