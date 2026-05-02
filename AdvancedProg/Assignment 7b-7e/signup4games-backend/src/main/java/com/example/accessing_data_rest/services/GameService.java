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

    // DONE Assignment 7b: create a game in the repository and return the result
    // DONE Assignment 7c: make sure that the game is created with the owner
    //      who must be in the repository already, and also with the owner as first player

    /** Creates a game in the repository and returns the result. The game must have at least 2 players, and the owner must be an existing user in the repository.
     * The owner is also added as the first player of the game.
     *
     * @param game The game to create. The owner must be an existing user in the repository, and the game must have at least 2 players and at max 6 players.
     * @return  The created game with the owner and the first player set to the owner.
     * @throws IllegalStateException If the game has less than 2 players or if the owner is not an existing user in the repository or if there are more than 6 players.
     */
    @Transactional
    public Game createGame(Game game) {
        try {
            User owner = userRepository.findByName(game.getOwner().getName()).getFirst();
            if (game.getMinPlayers() >= 2 && game.getMaxPlayers() <= 6 && owner != null) {

                game.setOwner(owner);  // Now game points to user

                Game savedGame = gameRepository.save(game);  // Game now has ID

                Player player = new Player();
                player.setGame(savedGame);   // Managed game
                player.setUser(owner);       // Managed user
                player.setName(owner.getName());
                playerRepository.save(player);

                return gameRepository.findByUid(savedGame.getUid()).get(0);

            } else {throw new IllegalStateException("Game must have at least 2 players and at max 6 players, and the owner must be an existing user in the repository.");}
        }
        catch (Exception e)
        {
            System.out.println(e.getMessage());
        }
        return null;

    }

    @Transactional
    public Game updateGameState(long gameUid, Game.GameState gameState) {
        try {
            Game existingGame = gameRepository.findByUid(gameUid).get(0);
            if (existingGame != null) {
                existingGame.setGameState(gameState);
                return gameRepository.save(existingGame);
            } else {
                throw new IllegalStateException("Game with uid " + existingGame.getUid() + " does not exist.");
            }
        } catch (Exception e) {
            System.out.println(e.getMessage());
        }
        return null;
        }

    @Transactional
    public void deleteGame(long gameUid){
        gameRepository.deleteById(gameUid);
    }

}
