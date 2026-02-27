package dk.dtu.compute.course02324.assignment3.lists.uses;

import dk.dtu.compute.course02324.assignment3.lists.implementations.GenericComparator;
import javafx.beans.value.ChangeListener;
import javafx.beans.value.ObservableValue;
import javafx.geometry.Insets;
import javafx.geometry.Pos;
import javafx.scene.control.Button;
import javafx.scene.control.Label;
import javafx.scene.control.ScrollPane;
import javafx.scene.control.TextField;
import javafx.scene.layout.GridPane;
import javafx.scene.layout.HBox;
import javafx.scene.layout.VBox;

import jakarta.validation.constraints.NotNull;

import java.util.Comparator;
import java.util.Map;

import java.util.List;
import java.util.stream.Collectors;

/**
 * A GUI element that is allows the user to interact and
 * change a list of persons.
 *
 * @author Ekkart Kindler, ekki@dtu.dk
 */
public class PersonsGUI extends GridPane {

    private static final String NO_MOST_OCCURRING_NAME = "No most occurring name";

    /**
     * The list of persons to be maintained in this GUI.
     */
    private final List<Person> persons;

    private GridPane personsPane;
    private Label avgWeight;
    private Label mostFrequentName;
    private Label maxAge;
    private Label minAge;

    /**
     * Constructor which sets up the GUI attached a list of persons.
     *
     * @param persons the list of persons which is to be maintained in
     *                this GUI component; it must not be <code>null</code>
     */
    public PersonsGUI(@NotNull List<Person> persons) {
        this.persons = persons;

        setVgap(5.0);
        setHgap(5.0);

        // text field for user entering a name
        TextField nField = new TextField();
        nField.setPrefColumnCount(8);
        nField.setPromptText("name");

        // text field for user entering the persons weight
        TextField wField = new TextField();
        wField.setPrefColumnCount(8);
        wField.setPromptText("weight");

        // text field for user entering the age of a person
        TextField aField = new TextField();
        aField.setPrefColumnCount(8);
        aField.setPromptText("age");

        // text field for entering index position
        TextField iField = new TextField();
        iField.setPrefColumnCount(8);
        iField.setPromptText("index");

        //labels for the fields
        Label name = new Label("Name");
        Label weightStr = new Label("Weight");
        Label ageLabel = new Label("Age");


        avgWeight = new Label("Average weight: 0.0");
        mostFrequentName = new Label("Most occurring name: " + NO_MOST_OCCURRING_NAME);

        maxAge = new Label("no max age");
        minAge = new Label("no min age");

        // button for adding a new person to the list (based on
        // the name and weight in the text fields)
        Button addButton = new Button("Add at the end of the list");
        addButton.setOnAction(e -> {
            try {
                Person person = createPerson(nField, wField,aField);
                persons.add(person);
                refreshStatistics();
                update();
            } catch (NumberFormatException ex) {
                System.out.println("(add at end button) Weight must be a number. Message: " + ex.getMessage());
            } catch (IllegalArgumentException ex) {
                System.out.println("A person must be initialized with a non-null name and a positive weight.");
            }
        });


        Button addAtIndexButton = new Button("Add at index:");
        addAtIndexButton.setOnAction(e -> {
            try {
                int index = Integer.parseInt(iField.getText().trim());
                Person person = createPerson(nField, wField, aField);
                persons.add(index, person);
                refreshStatistics();
                update();
            } catch (NumberFormatException ex) {
                System.out.println("(index button) Index and weight must be numbers. Message: " + ex.getMessage());
            } catch (IllegalArgumentException ex) {
                System.out.println("A person must be initialized with a non-null name and a positive weight.");
            } catch (IndexOutOfBoundsException ex) {
                System.out.println("The index is out of bounds");
            }
        });

        Comparator<Person> comparator = new GenericComparator<>();

        // button for sorting the list (according to the order of Persons,
        // which implement the interface Comparable, which is converted
        // to a Comparator by the GenericComparator above)
        Button sortButton = new Button("Sort");
        sortButton.setOnAction(e -> {
            try {
                persons.sort(comparator);
                // makes sure that the GUI is updated accordingly
                update();
            } catch (UnsupportedOperationException ex) {
                throw new RuntimeException("Cannot sort this: " + ex.getMessage());
            }
        });

        // button for clearing the list
        Button clearButton = new Button("Clear");
        clearButton.setOnAction(e -> {
            persons.clear();
            // makes sure that the GUI is updated accordingly
            update();
        });


        final double GAP = 8.0;

        VBox nameBox = new VBox(GAP, name, nField);
        VBox weightBox = new VBox(GAP, weightStr, wField);
        VBox ageBox = new VBox(GAP, ageLabel, aField);

        HBox inputRow = new HBox(GAP, nameBox, weightBox,ageBox);

        VBox indexFieldBox = new VBox(GAP, iField);
        VBox addAtIndexButtonBox = new VBox(GAP, addAtIndexButton);
        HBox indexRow = new HBox(GAP, addAtIndexButtonBox, indexFieldBox);

        // combines the above elements into vertically arranged boxes
        // which are then added to the left column of the grid pane
        VBox actionBox = new VBox(inputRow, addButton,indexRow, sortButton, clearButton, avgWeight, mostFrequentName,maxAge, minAge);
        actionBox.setSpacing(10.0);
        add(actionBox, 0, 0);

        // create the elements of the right column of the GUI
        // (scrollable person list) ...
        Label labelPersonsList = new Label("Persons:");

        personsPane = new GridPane();
        personsPane.setPadding(new Insets(5));
        personsPane.setHgap(5);
        personsPane.setVgap(5);

        ScrollPane scrollPane = new ScrollPane(personsPane);
        scrollPane.setMinWidth(300);
        scrollPane.setMaxWidth(300);
        scrollPane.setMinHeight(300);
        scrollPane.setMaxHeight(300);
        scrollPane.setVbarPolicy(ScrollPane.ScrollBarPolicy.AS_NEEDED);
        scrollPane.setHbarPolicy(ScrollPane.ScrollBarPolicy.AS_NEEDED);

        // ... and adds these elements to the right-hand columns of
        // the grid pane
        VBox personsList = new VBox(labelPersonsList, scrollPane);
        personsList.setSpacing(5.0);
        add(personsList, 1, 0);

        // updates the values of the different components with the values from
        // the stack
        update();
    }


    private Person createPerson(TextField nameField, TextField weightField, TextField ageField) {
        double weight = Double.parseDouble(weightField.getText().trim());
        Integer age = Integer.parseInt(ageField.getText().trim());
        return new Person(nameField.getText().trim(), weight, age);
    }

    private double averageWeight(List<Person> p) {
        if (p.isEmpty()) {
            return 0.0;
        }
        double total = 0;
        for (int i = 0; i < p.size(); i++) {
            total += p.get(i).weight;
        }

        return total / p.size();
    }


    private void refreshStatistics() {

        if(persons.isEmpty()){
            mostFrequentName.setText("no data available");
            avgWeight.setText("no data available");
            maxAge.setText("no data available");
            minAge.setText("no data available");
        } else {
            double averageWeight;
            averageWeight = persons.stream().mapToDouble(p -> p.weight).average().orElse(0.0);

            var mostFrequent = persons.stream()
                    .collect(Collectors.groupingBy(p -> p.name,
                            Collectors.summingInt(p -> 1)))
                    .entrySet().stream()
                    .max(Map.Entry.comparingByValue())
                    .orElse(null);


            mostFrequentName.setText("Most occurring name: " + mostFrequent.getKey() + " occurs: " + mostFrequent.getValue() + " times");
            avgWeight.setText(String.format("The average weight is: %.2f", averageWeight) + " kg");

            maxAge.setText("The max age is: " + persons.stream()
                    .map(p -> p.getAge())
                    .reduce(Integer :: max)
                    .orElse(0));

            minAge.setText("The min age is: " + persons.stream()
                    .map(p -> p.getAge())
                    .reduce(Integer :: min)
                    .orElse(0));
        }


    }

    private void update() {
        personsPane.getChildren().clear();
        // adds all persons to the list in the personsPane (with
        // a delete button in front of it)

        for (int i = 0; i < persons.size(); i++) {
            Person person = persons.get(i);
            Label personLabel = new Label(i + ": " + person.toString());
            Button deleteButton = new Button("Delete");
            deleteButton.setOnAction(e -> {
                persons.remove(person);
                update();
                refreshStatistics();
            });

            HBox entry = new HBox(deleteButton, personLabel);
            entry.setSpacing(5.0);
            entry.setAlignment(Pos.BASELINE_LEFT);
            personsPane.add(entry, 0, i);
        }
    }

}

class NonNegativeIntegerWatcher implements ChangeListener<String> {

    private TextField textField;

    private int init;

    /**
     * This constructor initializes the watcher on a text fields with some
     * initial value. Note that this constructor registers this watcher
     * as a {@link ChangeListener<String>} on the text field-
     *
     * @param textField the text field to watch over
     * @param init the initial value in the text field
     */
    public NonNegativeIntegerWatcher(@NotNull TextField textField, int init) {
        this.textField = textField;
        this.init = init;
        textField.setText(init + "");
        textField.textProperty().addListener(this);
    }

    @Override
    public void changed(ObservableValue<? extends String> observableValue, String oldValue, String newValue) {
        try {
            int i = Integer.parseInt(newValue);
            if (i >= 0) {
                textField.setText(newValue);
            } else {
                textField.setText(oldValue);
            }
        } catch (NumberFormatException e) {
            if (newValue.isEmpty()) {
                textField.setText("" + init);
            } else {
                textField.setText(oldValue);
            }
        }
    }

    /**
     * Returns the integer value currently represented in the watched text field.
     *
     * @return the integer value in the text field
     */
    public int getValue() {
        try {
            String text = textField.getText();
            int i = Integer.parseInt(text);
            if (i >= 0) {
                return i;
            }
        } catch (NumberFormatException e) {
        }
        return 0;
    }

}
