package dk.dtu.compute.course02324.assignment4.functions.uses;


import dk.dtu.compute.course02324.assignment4.functions.implementations.BubbleSort;
import dk.dtu.compute.course02324.assignment4.functions.implementations.GenericComparator;

import java.util.ArrayList;
import java.util.Comparator;
import java.util.List;

public class TestLists {

    public static Comparator<Person> comparator = new GenericComparator<>();

    public static void main(String[] args) {
        List<Person> persons = new ArrayList<>();

        persons.add(new Person("Egon", 50, 14));
        persons.add(new Person("Ekkart", 74, 56));
        persons.add(new Person("Anton", 84, 0));
        persons.add(new Person("Carlos", 70, 37));
        persons.add(new Person("Ekkart", 70, 52));

        print(persons);
        System.out.println("--------------------");
        persons.sort(comparator);
        // BubbleSort.sort(comparator,persons);

        print(persons);
        System.out.println("--------------------");

        persons.add(2, new Person("Xavi", 85, 122));

        print(persons);
        System.out.println("--------------------");
        // persons.sort(comparator);
        BubbleSort.sort(comparator, persons);

        print(persons);
        System.out.println("--------------------");

        persons.add(new Person("Egon", 50, 14));
        persons.add(new Person("Ekkart", 74, 56));
        persons.add(new Person("Anton", 84, 0));
        persons.add(new Person("Carlos", 70, 37));
        persons.add(new Person("Ekkart", 70, 52));

        print(persons);
        System.out.println("--------------------");

        // persons.add(0, new Person("Egon", 100 , 45)); // should throw an exception
        persons.add(new Person("Egon", 100 , 45));
        print(persons);
        System.out.println("--------------------");

        // persons.sort(comparator);  // should throw an exception
    }

    public static void print(List<?> list) {
        for (int i = 0; i < list.size(); i++ ) {
            System.out.println(i + ": " + list.get(i));
        }
    }

}
