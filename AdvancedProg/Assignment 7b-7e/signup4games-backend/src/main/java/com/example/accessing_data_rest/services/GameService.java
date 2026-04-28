package com.example.accessing_data_rest.services;

import com.example.accessing_data_rest.model.Game;
import com.example.accessing_data_rest.model.Player;
import com.example.accessing_data_rest.model.User;
import com.example.accessing_data_rest.repositories.GameRepository;
import com.example.accessing_data_rest.repositories.PlayerRepository;
import com.example.accessing_data_rest.repositories.UserRepository;
import jakarta.transaction.Transactional;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.stereotype.Service;
import org.springframework.web.bind.annotation.RequestBody;

import java.util.ArrayList;
import java.util.List;

@Service
public class GameService {

    @Autowired
    private GameRepository gameRepository;
    @Autowired
    private UserRepository userRepository;
    @Autowired
    private UserService userService;
    @Autowired
    private PlayerRepository playerRepository;

    public List<Game> getGames() {
        // DONE Assignment 7b: Implement the method for obtaining all games from the
        //      GameRepository (using finaAll) and returning it as a list
        List<Game> games = new ArrayList<>();
        gameRepository.findAll().forEach(games::add);
        return games;
    }

    // TODO Assignment 7b: create a game in the repository and return the result

    @Transactional
    public Game createGame(Game game) {

        User owner = userRepository.findByName(game.getOwner().getName()).get(0);

        game.setOwner(owner);  // Now game points to user

        Game savedGame = gameRepository.save(game);  // Game now has ID

        Player player = new Player();
        player.setGame(savedGame);   // Managed game
        player.setUser(owner);       // Managed user
        player.setName(owner.getName());
        playerRepository.save(player);

        return gameRepository.findByUid(savedGame.getUid()).get(0);
    }

    // TODO Assignment 7c: make sure that the game is created with the owner
    //      who must be in the repository already, and also with the owner as first player

}
