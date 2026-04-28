package com.example.accessing_data_rest.services;

import java.util.List;
import java.util.Optional;

import com.example.accessing_data_rest.exceptions.CouldNotCreatePlayerException;
import com.example.accessing_data_rest.model.Game;
import com.example.accessing_data_rest.model.Player;
import com.example.accessing_data_rest.model.User;
import com.example.accessing_data_rest.repositories.GameRepository;
import com.example.accessing_data_rest.repositories.PlayerRepository;
import com.example.accessing_data_rest.repositories.UserRepository;
import jakarta.transaction.Transactional;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.stereotype.Service;


@Service
public class PlayerService {

    @Autowired
    private PlayerRepository playerRepository;
    @Autowired
    private UserRepository userRepository;
    @Autowired
    private GameRepository gameRepository;


    @Transactional
    public Player signUpPlayer(Player player){

        List<Game> games = gameRepository.findByUid(player.getGame().getUid());
        List<User> users = userRepository.findByName(player.getUser().getName());

        if (games.isEmpty() || users.isEmpty()){
            throw new CouldNotCreatePlayerException("Could not find user or game");
        }

        Game game = games.get(0);
        User owner = users.get(0);

        if (game.getPlayers().size() >= game.getMaxPlayers()){
            throw new CouldNotCreatePlayerException("No more space in game. could not create user");
        }

        player.setGame(game);
        player.setUser(owner);

        return playerRepository.save(player);
    }

}
