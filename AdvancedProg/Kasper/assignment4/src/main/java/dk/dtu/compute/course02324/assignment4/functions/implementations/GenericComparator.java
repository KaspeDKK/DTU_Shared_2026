package dk.dtu.compute.course02324.assignment4.functions.implementations;

import java.util.Comparator;

/**
 * This is a generic comparator, which turns any comparable into a comparatir without any additional
 * programming.
 *
 * @param <T> the comparable to be turned into a comparator
 *
 * @author Ekkart Kindler, ekki@dtu.dk
 */
public class GenericComparator<T extends Comparable<T>> implements Comparator<T> {

    @Override
    public int compare(T o1, T o2) {
        if (o1 == null || o2 == null) {
            throw new IllegalArgumentException("Arguments of compare must (both) not be null");
        }
        return o1.compareTo(o2);
    }

}
