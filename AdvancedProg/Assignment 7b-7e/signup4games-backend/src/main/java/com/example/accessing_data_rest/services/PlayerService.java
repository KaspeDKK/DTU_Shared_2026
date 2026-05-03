package com.example.accessing_data_rest.services;

import java.util.List;
import java.util.Optional;

import com.example.accessing_data_rest.exceptions.*;
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


    /** Used for PlayerController to Post to an endpoint
     *
     * @param player to save to the repository after changes
     * @return a player with a game, its user and an id
     */
    @Transactional
    public Player signUpPlayer(Player player){

        List<Game> games = gameRepository.findByUid(player.getGame().getUid());
        List<User> users = userRepository.findByName(player.getUser().getName()); // names are unique

        if (games.isEmpty() || users.isEmpty()){
            throw new CouldNotCreatePlayerException("Could not find user or game");
        }

        Game game = games.get(0);
        User owner = users.get(0);

        if (game.getGameState() == Game.GameState.ACTIVE){
            throw new CannotJoinActiveGameException("Game is already active. could not create player");
        }

        if (game.getPlayers().size() >= game.getMaxPlayers()){
            throw new CouldNotCreatePlayerException("Can't join game. Game is full");
        }

        player.setGame(game);
        player.setUser(owner);

        return playerRepository.save(player);
    }

    /** this method deletes the player belonging to the given playerUid from the player repository.
     * Conditions for this deletion are the following:<p></p>
     *  - playerUid actually belongs to a player
     *  - the game hasn't been started yet
     *  - the playerUid doesn't belong to the owner of the game
     *
     * @param playerUid of the player that wishes to leave the game
     */
    @Transactional
    public void leaveGame(Long playerUid) {

        // is present check
        if (!playerRepository.existsById(playerUid)) {
            throw new CouldNotFindPlayerException("Could not find player");
        }

        Player player = playerRepository.findById(playerUid).get();

        if (player.getGame().getGameState() == Game.GameState.ACTIVE) {
            throw new CannotLeaveActiveGameException("Game is already active. Player could not leave game (delete player)");
        }

        // using userUid to check if this player is the owner
        if (player.getGame().getOwner().getUid() == (player.getUser().getUid())) {
            throw new CouldNotDeletePlayerException("Game owner cannot leave game");
        }

        playerRepository.deleteById(playerUid);
    }

}
