package dk.dtu.compute.course02324.assignment3.lists.uses;


import dk.dtu.compute.course02324.assignment3.lists.implementations.GenericComparator;
//import dk.dtu.compute.course02324.assignment3.lists.types.List;
import javafx.geometry.Insets;
import javafx.geometry.Pos;
import javafx.scene.control.*;
import javafx.scene.layout.GridPane;
import javafx.scene.layout.HBox;
import javafx.scene.layout.VBox;

import jakarta.validation.constraints.NotNull;

import java.util.*;
import java.util.stream.Collectors;

/**
 * A GUI element that is allows the user to interact and
 * change a list of persons.
 *
 * @author Ekkart Kindler, ekki@dtu.dk
 */
public class PersonsGUI extends GridPane {

    /**
     * The list of persons to be maintained in this GUI.
     */
    final private List<Person> persons;

    private GridPane personsPane;

    private TextArea textAreaExceptions;

    private Map<String, Integer> nameMap;


    //Weight

    private double averageWeight;
    private Label averageWeightAmount;
    private Label mostOccuringAmount;

    //Age
//    private Integer maxAge;
//    private Integer minAge;
    private Label maxAgeLabel;
    private Label minAgeLabel;


    /**
     * Constructor which sets up the GUI attached a list of persons.
     *
     * @param persons the list of persons which is to be maintained in
     *                this GUI component; it must not be <code>null</code>
     */
    public PersonsGUI(@NotNull java.util.List<Person> persons) {
        this.persons = persons;

//        this.count = 0.0;
//        this.totalWeight = 0.0;

        this.setVgap(5.0);
        this.setHgap(5.0);
        this.nameMap = new HashMap<>();


        // text filed for user entering a name
        TextField field = new TextField("name");
        field.setPrefColumnCount(5);

        //Egen implement:
        TextField wField = new TextField("weight");
        wField.setPrefColumnCount(5);

        //Egen implement af age:
        TextField aField = new TextField("Age");
        wField.setPrefColumnCount(5);

        //Egen implement af index:
        TextField indexField = new TextField("index");
        indexField.setPrefColumnCount(5);

        Label averageWeight = new Label("Average Weight:");
        this.averageWeightAmount = new Label("No info");

        Label mostOccuring = new Label("Most occuring name:");
        this.mostOccuringAmount = new Label("No info");

        this.averageWeight = 0.0;

        // age implementations

        Label maxAgeLabelPlaceholder = new Label("Maximum age:");
        this.maxAgeLabel = new Label("No info");

        Label minAgeLabelPlaceholder = new Label("Minimum age:");
        this.minAgeLabel = new Label("No info");



        // TODO for all buttons installed below, the actions need to properly
        //      handle (catch) exceptions, and it would be nice if the GUI
        //      could also show the exceptions thrown by user actions on
        //      button pressed (cf. Assignment 2).

        // button for adding a new person to the list (based on
        // the name in the text field (the weight is just incrementing)
        // TODO a text field for the weight could be added to this GUI


        Button addAtIndex = new Button("Add at index");
        addAtIndex.setOnAction(
                e -> {
                    try {
                        if (field.getText().isEmpty() ||aField.getText().isEmpty() || wField.getText().isEmpty()) {
                            throw new IllegalArgumentException("A person must have a name, weight and age \n");
                        }
                        int insertIndex = Integer.parseInt(indexField.getText());
                        Double weight = Double.parseDouble(wField.getText());
                        String name = field.getText();
                        Integer age = Integer.parseInt(aField.getText());
                        Person person = new Person(name, weight, age);
                        persons.add(insertIndex, person);

                    } catch (NumberFormatException ex) {
                        textAreaExceptions.appendText("(Add At Index) Input-ex:: " + ex.getMessage() + "\n");
                        // formenligt skelne mellem double index og weight exception
                    } catch (IndexOutOfBoundsException ex) {
                        textAreaExceptions.appendText("(Add At Index) IOOB-ex:: " + ex.getMessage() + "\n");
                    } catch (UnsupportedOperationException ex) {
                        textAreaExceptions.appendText(ex.getMessage());
                    } catch (IllegalArgumentException ex) {
                        textAreaExceptions.appendText(ex.getMessage());
                    }
                    finally {
                        refreshStats();
                        update();
                    }
                }
        );


        Button addButton = new Button("Add");
        addButton.setOnAction(
                e -> {
                    try {
                        if (field.getText().isEmpty() || wField.getText().isEmpty() || aField.getText().isEmpty()) {
                            throw new IllegalArgumentException("A person must have a name, weight and age \n");
                        }
                        if ((Integer.parseInt(aField.getText()) > 99 || Integer.parseInt(aField.getText()) < 0)){
                            throw new IllegalArgumentException("A person needs to have a reasonable age");
                        }
                        Double weight = Double.parseDouble(wField.getText());
                        String name = field.getText();
                        Integer age = Integer.parseInt(aField.getText());
                        Person person = new Person(name, weight, age);
                        persons.add(person);

                    } catch (NumberFormatException ex) {
                        textAreaExceptions.appendText("Wheight needs to be of type double insted of: " + ex.getMessage() + "\n");
                    }catch (IllegalArgumentException ex) {
                        textAreaExceptions.appendText(ex.getMessage() + "\n");
                    }  finally {
                        refreshStats();
                        update();
                    }
                });

        Comparator<Person> comparator = new GenericComparator<>();

        // button for sorting the list (according to the order of Persons,
        // which implement the interface Comparable, which is converted
        // to a Comparator by the GenericComparator above)
        Button sortButton = new Button("Sort");
        sortButton.setOnAction(
                e -> {
                    try {
                        persons.sort(comparator);
                    } catch (UnsupportedOperationException ex) {
                        textAreaExceptions.appendText("Can't sort this: " + ex.getMessage() + "\n");
                    }
                    finally {
                        // makes sure that the GUI is updated accordingly
                        update();
                    }
                });

        // button for clearing the list
        Button clearButton = new Button("Clear");
        clearButton.setOnAction(
                e -> {
                    persons.clear();
                    textAreaExceptions.setText("");

                    // makes sure that the GUI is updated accordingly
                    refreshStats();
                    update();
                });

        Button passTimeButton = new Button("Pass time");
        passTimeButton.setOnAction(
                e -> {
                    persons.stream().forEach(Person::passTime);

                    //update GUI
                    refreshStats();
                    update();
                }
        );

        // combines the above elements into vertically arranged boxes
        // which are then added to the left column of the grid pane


        // input area
        HBox actionBoxInput = new HBox(field, wField,aField);
        actionBoxInput.setSpacing(5.0);

        HBox actionBoxButtons = new HBox(addButton, sortButton, clearButton, passTimeButton);
        actionBoxButtons.setSpacing(5.0);

        VBox actionBox = new VBox(actionBoxInput, actionBoxButtons);
        actionBox.setSpacing(5.0);
        actionBox.setMinHeight(70);




        // input at index area:
        Label labelIndexInput = new Label("Inset at specified index:");

        HBox actionIndexInput = new HBox(addAtIndex, indexField);
        actionIndexInput.setSpacing(5.0);
        actionIndexInput.setMinHeight(70);

        VBox indexBox = new VBox(labelIndexInput, actionIndexInput);


        VBox combined = new VBox(actionBox, indexBox, averageWeight, averageWeightAmount, mostOccuring, mostOccuringAmount,maxAgeLabelPlaceholder, maxAgeLabel,minAgeLabelPlaceholder, minAgeLabel);
        combined.setPadding(new Insets(5));
        combined.setSpacing(5.0);
        this.add(combined, 0, 0);




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


        // Exception window
        Label labelExceptions = new Label("Exceptions:");
        textAreaExceptions = new TextArea();
        textAreaExceptions.setWrapText(true);
        textAreaExceptions.setText("");
        textAreaExceptions.setEditable(false);
        textAreaExceptions.setScrollTop(Double.MAX_VALUE);

        ScrollPane scrollPaneE = new ScrollPane(textAreaExceptions);
        scrollPaneE.setVbarPolicy(ScrollPane.ScrollBarPolicy.AS_NEEDED);
        scrollPaneE.setHbarPolicy(ScrollPane.ScrollBarPolicy.NEVER);
        textAreaExceptions.setMinWidth(300);
        textAreaExceptions.setMaxWidth(300);
        textAreaExceptions.setMinHeight(300);
        textAreaExceptions.setMaxHeight(300);



        // ... and adds these elements to the right-hand columns of
        // the grid pane
        HBox peLabelList = new HBox(labelPersonsList, labelExceptions);
        peLabelList.setSpacing(8.0);

        HBox peSpaceList = new HBox(scrollPane, scrollPaneE);
        peSpaceList.setSpacing(8.0);


        VBox personsList = new VBox(labelPersonsList, scrollPane);
        personsList.setSpacing(5.0);

        VBox exceptionList = new VBox(labelExceptions, scrollPaneE);
        exceptionList.setSpacing(5.0);

        HBox rightSide = new HBox(personsList, exceptionList);
        this.add(rightSide, 1, 0);
        rightSide.setSpacing(5.0);



        // updates the values of the different components with the values from
        // the stack
        update();
    }

    /**
     * Updates the values of the GUI elements with the current values
     * from the list.
     *
     * @Note: MostUsedName works like Catan-rules, if more than one name satisfy "most used", then the first name added, appears as most used.
     */
    private void update() {
//        count = 0;
//        totalWeight = 0;
        nameMap.clear();
        averageWeight = 0.0;


        personsPane.getChildren().clear();
        // adds all persons to the list in the personsPane (with
        // a delete button in front of it)
        for (int i=0; i < persons.size(); i++) {
            Person person = persons.get(i);
            Label personLabel = new Label(i + ": " + person.toString());
            Button deleteButton = new Button("Delete");
            deleteButton.setOnAction(
                    e -> {
                        persons.remove(person);
                        refreshStats();
                        update();
                    }
            );



            HBox entry = new HBox(deleteButton, personLabel);
            entry.setSpacing(5.0);
            entry.setAlignment(Pos.BASELINE_LEFT);
            personsPane.add(entry, 0, i);

            //removes persons
            persons.removeIf(p -> p.getAge() > 98);
        }
    }


    /**
     * This method refreshed averege weight, and most occuing name. This is a specific
     * method outside of the "update method", because sort shouldn't make theese calculations,
     * only addding and deleting should call this method.
     */
    private void refreshStats() {
//        totalWeight = 0;
        nameMap.clear();
        averageWeight = 0.0;



        //4.A Implement Lambda function instead.
        averageWeight = persons.stream().map(Person::getWeight)
                .reduce(0.0,(w1,w2) -> w1 + w2);


        // average-weight calc:
        if (persons.isEmpty() == true) {
            averageWeightAmount.setText("No info");
        } else {
            averageWeightAmount.setText(String.format("%.2f", averageWeight/persons.size()) + " kg");
        }


        // Most used name lambda
        nameMap = persons.stream()      //her "lægger jeg alle objecter af person på et bånd"
                .collect(Collectors.groupingBy(         //Collect samler det i et nyt map tror jeg, hvor collectors.groupingBy er sorteringsmekanismen
                        person -> person.getName(), Collectors.summingInt(person -> 1)     //hvor ve side er <key> og højre side summer op <value>
                ));

        // så jeg ender med et map<String, Integer> hvor <Ekkart, 4> f.eks. betyder ekkart navnet dukker op 4 gange.
        // Nu har jeg mine værdier, så skal jeg finde "max" value.
        nameMap.entrySet().stream() // jeg lægger parne af <key, value> ud på et bånd.
                .max(Comparator.comparingInt(entry -> entry.getValue())) // max sammenligning på <value>
                .ifPresentOrElse(
                        entry -> mostOccuringAmount.setText(entry.getValue() + "* " + entry.getKey()),
                        () ->  mostOccuringAmount.setText("No info")
                );


        //Finder max og min af Person age.
        int minAge = persons.stream() //Stream undgår loop
                .map(Person::getAge) //vi mapper person og bruger vores funktion getAge (Person)
                .reduce(999, (a1, a2) ->
                        a1 < a2 ? a1 : a2);  // 999 som start værdi - vi reducere ned til den mindste værdi via en lambda funktion.
        minAgeLabel.setText("Min age: " + minAge);

        int maxAge = persons.stream().map(Person::getAge)
                .reduce(-1, (a1, a2) ->
                        a1 > a2 ? a1 : a2);  // -1 som start værdi istedet.
        maxAgeLabel.setText("Max age: " + maxAge);


        if (persons.isEmpty()) {
            mostOccuringAmount.setText("No info");
            maxAgeLabel.setText("No info:");
            minAgeLabel.setText("No info:");
        }



    }



    // TODO this GUI could be extended by some additional widgets for issuing other
    //      operations of lists. And the possibly thrown exceptions should be caught
    //      in the event handler (and possibly shown in an additional text area for
    //      exceptions; see Assignment 2).

}


