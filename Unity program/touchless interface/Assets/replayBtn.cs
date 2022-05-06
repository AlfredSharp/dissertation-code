using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class replayBtn : MonoBehaviour
{
    public Animator anim;

    // Update is called once per frame
    public void replay(string stateName){
        anim.Play(stateName, 0, 0.25f);
    }
}
