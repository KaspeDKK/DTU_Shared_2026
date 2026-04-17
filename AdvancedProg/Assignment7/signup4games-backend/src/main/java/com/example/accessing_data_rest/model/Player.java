package com.example.accessing_data_rest.model;

import jakarta.persistence.*;


@Entity
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

    public User getUser() {return user;}

    public void setUser(User user) {this.user = user;}
}
