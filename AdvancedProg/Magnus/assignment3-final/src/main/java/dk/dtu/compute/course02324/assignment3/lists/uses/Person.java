package dk.dtu.compute.course02324.assignment3.lists.uses;

import jakarta.validation.constraints.NotNull;

import java.util.Objects;

public class Person implements Comparable<Person> {

    final public String name;

    private double weight;

    private Integer age;

    Person(@NotNull String name, @NotNull double weight, @NotNull Integer age) {
        if (name == null || weight <= 0) {
            throw new IllegalArgumentException("A persons must be initialized with a" +
                    "(non null) name and an weight greater than 0");
        }

        this.age = age;
        this.name = name;
        this.weight = weight;
    }

    @Override
    public int compareTo(@NotNull Person o) {
        if (o == null) {
            throw new IllegalArgumentException("Argument of compareTo() must not be null");
        }
        if (this.name.compareTo(o.name) > 0) {
            return 1;
        }
        if (this.name.compareTo(o.name) < 0) {
            return -1;
        }
        // Here names are equal, so == 0, now compare weight.

        if (this.weight > o.weight) {
            return 1;
        }
        if (this.weight < o.weight) {
            return -1;
        }

        if (this.age > o.age) {
            return 1;
        } if (this.age < o.age) {
            return -1;
        }

        // both names and weight and age are equal.
        return 0;
    }

    /**
     * Computes a simple string representation of this object.
     *
     * @return a string representation of this object
     */
    @Override
    public String toString() {
        // This could be automatically generated, but this automatically
        // generated representation is a bit too verbose. Therefore, we
        // chose a simpler representation here.
        return name + ", " + weight + "kg" + ", " + age + " years";
    }


    public Integer getAge() {
        return this.age;
    }

    public void setAge(int newAge){
        this.age = newAge;
    }

    public void passTime(){
        this.age++;

        if (this.age > 30){
            this.weight = this.weight*1.08;
        }
    }

    public String getName() {
        return this.name;
    }

    public Double getWeight(){
        return this.weight;
    }

    @Override
    public boolean equals(Object o) {
        if (o == null || this.getClass() != o.getClass()) {
            return false;
        }

        Person person = (Person) o;
        if (this.compareTo(person) == 0) {
            return true;
        }
        return false;
    }

    @Override
    public int hashCode() {
        return Objects.hash(name, weight);
    }
}