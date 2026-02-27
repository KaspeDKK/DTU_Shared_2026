package dk.dtu.compute.course02324.assignment4.functions.implementations;

import dk.dtu.compute.course02324.assignment4.functions.types.List;

import jakarta.validation.constraints.NotNull;
import java.util.Comparator;

// TODO Assignment 4a: delete this file! We use Java standard Collections now

/**
 * An implementation of the interface {@link List} based on basic Java
 * arrays, which dynamically are adapted in size when needed.
 *
 * @param <E> the type of the list's elements.
 */
public class ArrayList<E> implements List<E> {

    /**
     * Constant defining the default size of the array when the
     * list is created. The value can be any (strictly) positive
     * number. Here, we have chosen <code>10</code>, which is also
     * Java's default for some array-based collection implementations.
     */
    final private int DEFAULT_SIZE = 10;

    /**
     * Current size of the list.
     */
    private int size = 0;

    /**
     *  The array for storing the elements of the
     */
    private E[] list = createEmptyArray(DEFAULT_SIZE);

    @Override
    public void clear() {
        size = 0;
    }

    @Override
    public int size() {
        return size;
    }

    @Override
    public @NotNull E get(int pos) throws IndexOutOfBoundsException {
        throw new UnsupportedOperationException("This operation is not yet implemented!");
        // TODO needs implementation (Assignment 3a)
    }

    @Override
    public E set(int pos, @NotNull E e) throws IndexOutOfBoundsException {
        throw new UnsupportedOperationException("This operation is not yet implemented!");
        // TODO needs implementation (Assignment 3a)
    }

    @Override
    public boolean add(@NotNull E e) {
        throw new UnsupportedOperationException("This operation is not yet implemented!");
        // TODO needs implementation (Assignment 3a)
    }

    @Override
    public boolean add(int pos, @NotNull E e) throws IndexOutOfBoundsException {
        throw new UnsupportedOperationException("This operation is not yet implemented!");
        // TODO needs implementation (Assignment 3a)
    }

    @Override
    public E remove(int pos) throws IndexOutOfBoundsException {
        throw new UnsupportedOperationException("This operation is not yet implemented!");
        // TODO needs implementation (Assignment 3a)
    }

    @Override
    public boolean remove(E e) {
        throw new UnsupportedOperationException("This operation is not yet implemented!");
        // TODO needs implementation (Assignment 3a)
    }

    @Override
    public int indexOf(E e) {
        throw new UnsupportedOperationException("This operation is not yet implemented!");
        // TODO needs implementation (Assignment 3a)
    }

    @Override
    public void sort(@NotNull Comparator<? super E> c) throws UnsupportedOperationException {
        throw new UnsupportedOperationException("This operation is not yet implemented!");
        // TODO needs implementation (Assignment 3b)
    }

    /**
     * Creates a new array of type <code>E</code> with the given size.
     *
     * @param length the size of the array
     * @return a new array of type <code>E</code> and the given length
     */
    private E[] createEmptyArray(int length) {
        // there is unfortunately no really easy and elegant way to initialize
        // an array with a type coming in as a generic type parameter, but
        // the following is simple enough. And it is OK, since the array
        // is never passed out of this class.
        return (E[]) new Object[length];
    }

    // TODO probably some private helper methods here (avoiding duplicated code)
    //      (Assignment 3a)

}
