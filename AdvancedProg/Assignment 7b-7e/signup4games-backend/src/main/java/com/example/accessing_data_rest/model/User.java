package com.example.accessing_data_rest.model;

import com.fasterxml.jackson.annotation.JsonIdentityInfo;
import com.fasterxml.jackson.annotation.ObjectIdGenerators;
import jakarta.persistence.*;

import java.util.List;

@Entity
@Table(name="user_table") // this is important! "user" is a keyword in H2 and not an identifier
@JsonIdentityInfo(
        scope=User.class,
        generator = ObjectIdGenerators.PropertyGenerator.class,
        property = "uid")
public class User {

    @Id
    @Column(name = "user_id")
    @GeneratedValue(strategy = GenerationType.AUTO)
    private long uid;

    private String name;

    // DONE Assignment 7a: this class needs to be extended with references to Player and
    //      the other way round (similar to the reference from Game to Player
    //      and the other way round.
    @OneToMany(mappedBy="user")
    private List<Player> players; // -> connection player

    public long getUid() {
        return uid;
    }

    public void setUid(long uid) {
        this.uid = uid;
    }

    public String getName() {
        return name;
    }

    public void setName(String name) {
        this.name = name;
    }

    // DONE Assignment 7a: this class needs to be extended with references to Player and
    //      the other way round (similar to the reference from Game to Player
    //      and the other way round (corresponding getter and setter).
    public List<Player> getPlayers() {
        return players;
    }

    public void setPlayers(List<Player> players) {
        this.players = players;
    }

}
