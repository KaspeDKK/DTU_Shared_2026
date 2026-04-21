package com.example.accessing_data_rest.model;

import com.fasterxml.jackson.annotation.JsonIdentityInfo;
import com.fasterxml.jackson.annotation.ObjectIdGenerators;
import jakarta.persistence.*;


@Entity
@JsonIdentityInfo(
        scope=Player.class,
        generator = ObjectIdGenerators.PropertyGenerator.class,
        property = "uid")
public class Player {

    // FIXME the ID of this could actually be the two foreign keys game_id and
    //       user_id, but this is a bit tricky to start with. So this will
    //       Not be done in the context of course 02324!
    @Id
    @Column(name="player_id")
    @GeneratedValue(strategy = GenerationType.AUTO)
    private long uid;

    private String name;

    @ManyToOne
    @JoinColumn
    private Game game;

    // DONE Assignment 7a: Add the reference to the user
    @ManyToOne
    @JoinColumn
    private User user;

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

    public Game getGame() {
        return game;
    }

    public void setGame(Game game) {
        this.game = game;
    }

    // DONE Assignment 7a: added the reference to the user
    //      (corresponding getter and setter).
    public User getUser() {
        return user;
    }

    public void setUser(User user) {
        this.user = user;
    }

}
