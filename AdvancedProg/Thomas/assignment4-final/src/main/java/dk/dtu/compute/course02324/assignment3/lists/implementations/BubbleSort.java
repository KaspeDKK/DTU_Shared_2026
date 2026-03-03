package dk.dtu.compute.course02324.assignment3.lists.implementations;

//import dk.dtu.compute.course02324.assignment3.lists.types.List;

import jakarta.validation.constraints.NotNull;
import java.util.Comparator;
import java.util.List;
import java.util.ArrayList;

/**
 * This is implementing a simplistic sorting algorithm based on the
 * Bubble Sort algorithm in a generic way. The actual sorting algorithm
 * is implemented as generic static method.
 */
public class BubbleSort {

    /**
     * Generic method for sorting a list of type Y according to a comparator.
     * The implementation is based on the BubbleSort algorithm, shown
     * in the lectures of the course Advanced Programming (02324) and
     * Project in Software Development (02362), adjusted from arrays to
     * lists and using the comparator instead of direct comparison of
     * integer values.
     *
     * @param comp the comparator
     * @param list the list
     * @param <T>  the type of the list
     */
    public static <T> void sort(@NotNull Comparator<? super T> comp, @NotNull List<T> list) {
        if (comp == null || list == null) {
            throw new IllegalArgumentException("Arguments of the sort function must not be null.");
        }
        boolean swapped;
        int j = list.size();
        do {
            swapped = false;
            for (var i = 0 ; i+1 < j ; i++) {
                T leftElement = list.get(i);
                T rightElement = list.get(i + 1);

                if (comp.compare(leftElement, rightElement) > 0) {
                    list.set(i, rightElement);
                    list.set(i + 1, leftElement);
                    swapped = true;
                }
            }
            j--;
        } while(swapped);
    }
}
