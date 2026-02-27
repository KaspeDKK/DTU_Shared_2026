package dk.dtu.compute.course02324.assignment4.functions.uses;

import javax.validation.constraints.NotNull;
import java.util.Objects;

/**
 * Person ADT for the assignments of 02324
 *
 * @author Ekkart Kindler, ekki@dtu.dk
 * @author Carlos E. Budde, cesbu@dtu.dk
 */
public class Person implements Comparable<Person> {

    final public String name;

    final public double weight;

    // TODO Assignment 4a:
    //  add a non-final Integer age field with setters and getters, viz. setAge(...) and getAge(...)

    Person(@NotNull String name,
           @NotNull double weight,
           @NotNull Integer age) {
        if (name == null || name.length() < 1)
            throw new IllegalArgumentException("Person must have a name (none given)");
        if (weight <= 0.0)
            throw new IllegalArgumentException("Person weight must be positive (\""+weight+"\" given)");
        if (age < 0)
            throw new IllegalArgumentException("Person age must be non-negative (\""+age+"\" given)");
        this.name = name;
        this.weight = weight;
    }


    @Override
    public int compareTo(@NotNull Person o) {
        if (o == null) {
            throw new IllegalArgumentException("Argument of compareTo() must not be null");
        }

        int result = this.name.compareTo(o.name);
        if (result != 0) {
            return result;
        }

        return (int) Math.signum(this.weight - o.weight);
    }

    /**
     * Computes a simple string representation of this object.
     *
     * @return a string representation of this object
     */
    @Override
    public String toString() {
        return name + ", " + weight + "kg";
    }

    /*
     * The following two methods must be implemented in order to respect the contract of objects
     * with respect to equals(), hashCode() and compareTo() methods. The details and reasons
     * as to why will be discussed much later.
     *
     * The implementations have been done fully automatically (using the two attributes
     * of person).
     *
     */

    @Override
    public boolean equals(Object o) {
        if (this == o) return true;
        if (o == null || getClass() != o.getClass()) return false;
        Person person = (Person) o;
        return Objects.equals(name, person.name) &&
                Double.compare(weight, person.weight) == 0 ;
    }

    @Override
    public int hashCode() {
        return Objects.hash(name, weight);
    }


}